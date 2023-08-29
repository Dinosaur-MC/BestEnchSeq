#include "tablemanager.h"
#include "core/fileoperator.h"

TableManager global_table_mgr;

TableManager::TableManager(QObject *parent)
    : QObject{parent}
{
    cursor = -1;
}

void TableManager::scanTable()
{
    this->blockSignals(true);
    table_list.clear();
    loadInternalTable();

    QDir dir("./");
    if (!dir.exists(PATH_TABLES))
    {
        qDebug() << dir.mkdir(PATH_TABLES);
        return;
    }
    dir.cd(PATH_TABLES);

    QStringList file_list = dir.entryList({"*.json", "*.csv", "*.json.disabled", "*.csv.disabled"}, QDir::Files);
    if (!file_list.isEmpty())
    {
        foreach (const auto &fname, file_list)
            loadTable(PATH_TABLES + fname);
    }
    cursor = -1;

    this->blockSignals(false);
    emit cursorChanged();
    emit tableUpdated();
}

void TableManager::disableTable(QString fname)
{
    int p = findTable(fname);
    if (p != -1)
    {
        QFile file(fname);
        if (!file.exists())
        {
            table_list.removeAt(p);
            return;
        }

        if (file.rename(fname + ".disabled"))
        {
            DataTable table;
            table.info.file_name = fname;
            table_list[p] = table;
            emit tableUpdated();
        }
    }
}

void TableManager::enableTable(QString fname)
{
    int p = findTable(fname);
    if (p != -1)
    {
        QFile file(fname);
        if (file.rename(fname.remove(fname.indexOf(".disabled"), QString(".disabled").size())))
        {
            reloadTable(p);
            emit tableUpdated();
        }
    }
}

bool TableManager::newTable(QString fname, int tmp_table)
{
    if (findTable(fname) != -1)
        return false;

    DataTable table;
    if (checkCursor(tmp_table))
        table = table_list.at(tmp_table);
    table.info.file_name = fname;

    table_list.append(table);
    cursor = table_list.size() - 1;

    emit cursorChanged();
    emit tableUpdated();
    return true;
}

bool TableManager::loadInternalTable()
{
    return loadTable(FILE_DEFAULT_TABLE);
}
bool TableManager::loadTable(QString fname)
{
    DataTable table;
    if (findTable(fname) != -1 || !FileOperator::loadTableData(fname, table))
        return false;

    table_list.append(table);
    cursor = table_list.size() - 1;

    emit cursorChanged();
    emit tableUpdated();
    return true;
}
void TableManager::unloadTable(QString fname)
{
    int p = findTable(fname);
    if (p != -1)
    {
        table_list.removeAt(p);
        if (cursor == p)
            cursor = -1;
        else if (cursor > p)
            cursor--;

        emit cursorChanged();
        emit tableUpdated();
    }
}

bool TableManager::reloadTable(int index)
{
    if (!checkCursor(index))
        return false;

    DataTable table;
    if (!FileOperator::loadTableData(table_list.at(index).info.file_name, table))
        return false;

    table_list[index] = table;
    emit tableUpdated();
    return true;
}

int TableManager::count() const
{
    return table_list.size();
}
void TableManager::empty()
{
    table_list.clear();
    cursor = -1;

    emit cursorChanged();
    emit tableUpdated();
}

void TableManager::setCursor(int num)
{
    cursor = num;
    emit cursorChanged();
}

QList<DataTableInfo> TableManager::getAllTabeInfo() const
{
    QList<DataTableInfo> info_list;
    foreach (auto &table, table_list)
        info_list.append(table.info);
    return info_list;
}
DataTableInfo TableManager::getTabeInfo() const
{
    if (!checkCursor())
        return DataTableInfo();
    return table_list.at(cursor).info;
}
DataTable TableManager::getTable() const
{
    if (!checkCursor())
        return DataTable();
    return table_list.at(cursor);
}
void TableManager::optimize()
{
    if (!checkCursor())
        optimizeAll();

    for (int i = 0; i < table_list.at(cursor).groups.size(); i++)
    {
        // 清理空组
        if (table_list[cursor].groups[i].enchantments.isEmpty())
        {
            table_list[cursor].groups.removeAt(i);
            i--;
            continue;
        }

        // 为无图标的组添加默认图标
        QIcon icon(table_list[cursor].groups[i].icon_path);
        if (icon.isNull() || icon.pixmap(QSize(48, 48)).isNull())
            table_list[cursor].groups[i].icon_path = ":/icon/weapon.png";
    }

    table_list[cursor].convertEnchTable();
    emit tableUpdated();
}
void TableManager::optimizeAll()
{
    int _cursor = cursor;

    this->blockSignals(true);
    for (cursor = 0; cursor < table_list.size(); cursor++)
        optimize();
    this->blockSignals(false);

    cursor = _cursor;
    emit tableUpdated();
}

void TableManager::insertEnch(int p, const EnchData &ench)
{
    if (!checkCursor())
        return;
    table_list[cursor].enchs.insert(p, ench);
    emit tableContentChanged(0, p);
}
void TableManager::replaceEnch(int p, const EnchData &ench)
{
    if (!checkCursor())
        return;
    table_list[cursor].enchs[p] = ench;
    emit tableContentChanged(0, p);
}
EnchData TableManager::takeEnch(int p)
{
    if (!checkCursor())
        return EnchData();
    if (!(p >= 0 && p < table_list.at(cursor).enchs.size()))
        return EnchData();

    emit tableContentChanged(0, p);
    return table_list[cursor].enchs.takeAt(p);
}

void TableManager::insertGroup(int p, const Group &group)
{
    if (!checkCursor())
        return;
    table_list[cursor].groups.insert(p, group);
    emit tableContentChanged(1, p);
}
void TableManager::replaceGroup(int p, const Group &group)
{
    if (!checkCursor())
        return;
    table_list[cursor].groups[p] = group;
    emit tableContentChanged(1, p);
}
Group TableManager::takeGroup(int p)
{
    if (!checkCursor())
        return Group();
    if (!(p >= 0 && p < table_list.at(cursor).groups.size()))
        return Group();

    emit tableContentChanged(1, p);
    return table_list[cursor].groups.takeAt(p);
}

bool TableManager::exportTable(QString path)
{
    if (!checkCursor())
        return false;

    optimize();
    return FileOperator::saveTableData(table_list.at(cursor), path);
}
void TableManager::unloadTable()
{
    if (checkCursor())
    {
        table_list.removeAt(cursor);
        cursor = -1;

        emit cursorChanged();
        emit tableUpdated();
    }
}
bool TableManager::apply()
{
    if (!checkCursor())
        return false;

    optimize();
    CT = table_list.at(cursor);
    return true;
}

int TableManager::findTable(QString fname) const
{
    for (int i = 0; i < table_list.size(); i++)
    {
        if (table_list.at(i).info.file_name == fname)
            return i;
    }
    return -1;
}

inline bool TableManager::checkCursor() const
{
    return checkCursor(cursor);
}
inline bool TableManager::checkCursor(int cur) const
{
    return -1 < cur && cur < table_list.size();
}
