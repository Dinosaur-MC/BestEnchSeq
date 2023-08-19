#include "tablemanager.h"
#include "core/fileoperator.h"

TableManager global_table_mgr;

TableManager::TableManager(QObject *parent)
    : QObject{parent}
{
    cursor = -1;
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

    return true;
}

bool TableManager::loadDefaultTable()
{
    return loadTable(FILE_DEFAULT_TABLE);
}
bool TableManager::loadTable(QString fname)
{
    DataTable table;
    if (findTable(fname) != -1 || !FileOperator::loadTableData("./tables/" + fname, table))
        return false;

    table_list.append(table);
    cursor = table_list.size() - 1;
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
    }
}

int TableManager::loadedCount()
{
    return table_list.size();
}
void TableManager::empty()
{
    table_list.clear();
}

void TableManager::setCursor(int num)
{
    cursor = checkCursor(num) ? num : -1;
}

DataTable TableManager::getTable()
{
    if (!checkCursor())
        return DataTable();
    return table_list.at(cursor);
}
void TableManager::optimize()
{
    if (!checkCursor())
        optimizeAll();

    foreach (auto &e, table_list.at(cursor).enchs)
    {
        foreach (auto &g, e.groups)
        {
            if (!table_list.at(cursor).groups.contains(Group(g)))
                table_list[cursor].groups.append(g);
        }
    }

    foreach (auto &g, table_list.at(cursor).groups)
    {
        const_cast<Group &>(g).enchantments.clear();
        foreach (auto &e, table_list.at(cursor).enchs)
        {
            if (e.groups.contains(g.name))
                const_cast<Group &>(g).enchantments.insert(e);
        }
    }

    for (int i = 0; i < table_list.at(cursor).groups.size(); i++)
    {
        if (table_list[cursor].groups[i].enchantments.isEmpty())
        {
            table_list[cursor].groups.removeAt(i);
            i--;
        }

        QIcon icon(table_list[cursor].groups[i].icon_path);
        if (icon.isNull())
            table_list[cursor].groups[i].icon_path = "";
    }

    table_list[cursor].convertEnchTable();
}
void TableManager::optimizeAll()
{
    int _cursor = cursor;
    for (cursor = 0; cursor < table_list.size(); cursor++)
        optimize();
    cursor = _cursor;
}

void TableManager::insertEnch(int p, EnchData ench)
{
    if (!checkCursor())
        return;
    table_list[cursor].enchs.insert(p, ench);
}
EnchData TableManager::takeEnch(int p)
{
    if (!checkCursor())
        return EnchData();
    if (!(p >= 0 && p < table_list.at(cursor).enchs.size()))
        return EnchData();
    return table_list[cursor].enchs.takeAt(p);
}
void TableManager::insertGroup(int p, Group group)
{
    if (!checkCursor())
        return;
    table_list[cursor].groups.insert(p, group);
}
Group TableManager::takeGroup(int p)
{
    if (!checkCursor())
        return Group();
    if (!(p >= 0 && p < table_list.at(cursor).groups.size()))
        return Group();
    return table_list[cursor].groups.takeAt(p);
}

bool TableManager::exportTable(QString path)
{
    if (!checkCursor())
        return false;

    optimize();
    return FileOperator::saveTableData(table_list.at(cursor), path);
}
bool TableManager::apply()
{
    if (!checkCursor())
        return false;

    optimize();
    current_table = table_list.at(cursor);
    return true;
}
void TableManager::unloadTable()
{
    if (checkCursor())
    {
        table_list.removeAt(cursor);
        cursor = -1;
    }
}

int TableManager::findTable(QString fname)
{
    for (int i = 0; i < table_list.size(); i++)
    {
        if (table_list.at(i).info.file_name == fname)
            return i;
    }
    return -1;
}
inline bool TableManager::checkCursor()
{
    return checkCursor(cursor);
}
inline bool TableManager::checkCursor(int cur)
{
    return cur >= 0 && cur < table_list.size();
}
