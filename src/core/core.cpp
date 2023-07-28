#include "core.h"

QVector<EnchData> global_u_ench_table;
QVector<_EnchData> global_p_ench_table;
QSet<Group> global_groups;
QMap<QString, QVariant> global_settings;
QVector<DataTableInfo> table_list;
DataTableInfo current_table;

QString intToRoman(int num)
{
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    QString reps[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    QString res;
    for (int i = 0; i < 13; i++)
    {
        while (num >= values[i])
        {
            num -= values[i];
            res += reps[i];
        }
    }
    return res;
}

QString MCEToString(MCE mce)
{
    if (mce == MCE::Java)
        return "Java";
    else if (mce == MCE::Bedrock)
        return "Bedrock";
    else
        return "Null";
}
MCE StringToMCE(QString str)
{
    if (str == "Java")
        return MCE::Java;
    else if (str == "Bedrock")
        return MCE::Bedrock;
    else
        return MCE::Null;
}

Ench::Ench(const _Ench &e)
{
    if (e.id < global_u_ench_table.size() && e.id >= 0)
    {
        this->name = global_u_ench_table.at(e.id).name;
        this->mce = global_u_ench_table.at(e.id).editions;
        this->lvl = e.lvl;
    }
    else
    {
        this->name = "@Unknown";
        this->mce.insert(MCE::Null);
        this->lvl = 0;
    }
}
bool Ench::isValid()
{
    if (this->name != "@Unknown" && this->lvl > 0 && !this->mce.contains(MCE::Null))
        return true;
    else
        return false;
}
bool operator==(const Ench &e1, const Ench &e2)
{
    if (e1.name == e2.name && e1.lvl == e2.lvl)
        return true;
    else
        return false;
}
bool operator<(const Ench &e1, const Ench &e2)
{
    if ((_Ench)e1 < (_Ench)e2)
        return true;
    else
        return false;
}
uint qHash(const Ench &key, uint seed)
{
    return qHash(key.name, seed) ^ key.lvl;
}

_Ench::_Ench(const Ench &e)
{
    this->id = -1;
    this->lvl = 0;

    for (int i = 0; i < global_u_ench_table.size(); i++)
    {
        if (global_u_ench_table.at(i).name == e.name && global_u_ench_table.at(i).editions == e.mce)
        {
            this->id = i;
            this->lvl = e.lvl;
        }
    }
}
bool _Ench::isValid()
{
    if (this->id >= 0 && this->lvl > 0)
        return true;
    else
        return false;
}
bool operator==(const _Ench &e1, const _Ench &e2)
{
    if (e1.id == e2.id && e1.lvl == e2.lvl)
        return true;
    else
        return false;
}
bool operator<(const _Ench &e1, const _Ench &e2)
{
    if ((e1.id < e2.id) || (e1.id == e2.id && e1.lvl < e2.lvl))
        return true;
    else
        return false;
}
uint qHash(const _Ench &key, uint seed)
{
    return qHash(key.id, seed) ^ key.lvl;
}

bool EnchData::isValid()
{
    if (this->name != "@Unknown" && this->max_lvl > 0 && this->book_multiplier > 0 && this->item_multiplier > 0 && this->editions.contains(MCE::Null) == false)
        return true;
    else
    {
        qDebug() << "Invalid Enchantment: " << this->name;
        return false;
    }
}
bool _EnchData::isValid()
{
    if (this->max_lvl > 0 && this->book_multiplier > 0 && this->item_multiplier > 0 && this->editions.contains(MCE::Null) == false)
        return true;
    else
        return false;
}

bool operator==(const EnchData &e1, const EnchData &e2)
{
    if (e1.name == e2.name && e1.editions == e2.editions)
        return true;
    else
        return false;
}
uint qHash(const EnchData &key, uint seed) // EnchData 仅以名称和版本(editions)区分
{
    uint hash = seed;

    hash ^= qHash(key.name) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    for (const auto &edition : key.editions)
    {
        hash ^= qHash(static_cast<int>(edition)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    return hash;
}

QVector<_EnchData> convertEnchTable(QVector<EnchData> &table) // 数据容器转换
{
    QVector<_EnchData> _table;
    for (int i = 0; i < table.size(); i++)
    {
        _EnchData ed;
        ed.max_lvl = table.at(i).max_lvl;
        ed.poor_max_lvl = table.at(i).poor_max_lvl;
        ed.book_multiplier = table.at(i).book_multiplier;
        ed.item_multiplier = table.at(i).item_multiplier;
        ed.editions = table.at(i).editions;
        _table.append(ed);
    }
    for (int i = 0; i < table.size(); i++)
    {
        for (auto &conf : table.at(i).conflictions)
        {
            int n = -1;
            QString name = conf;
            for (int j = 0; j < global_u_ench_table.size(); j++)
            {
                if (global_u_ench_table.at(j).name == name)
                {
                    n = j;
                    break;
                }
            }
            if (n >= 0)
            {
                _table[i].conflictions.insert(n);
            }
        }
    }
    return _table;
}

bool operator==(const Group &g1, const Group &g2)
{
    if (g1.name == g2.name)
        return true;
    else
        return false;
}
uint qHash(const Group &key, uint seed)
{
    return qHash(key.name, seed);
}

void registerSettings()
{
    global_settings.clear();
    global_settings["version/file_version"] = FILEVERSION;
    global_settings["version/app_version"] = VERSION_ID;
    global_settings["default/edition"] = 1;
    global_settings["default/item_config"] = 0;
    global_settings["default/algorithm"] = 0;
    global_settings["default/export_path"] = "./exports";
    global_settings["default/language"] = "zh_cn";
    global_settings["lever/auto_save"] = 0;
    global_settings["lever/auto_check_update"] = 1;
    global_settings["lever/enable_widely_reszie_window"] = 0;
    global_settings["lever/deverloper_mode"] = 0;
    global_settings["log/last_used_table"] = "";
    global_settings["log/last_edit"] = QDateTime::currentDateTime();
}

void initializeGlobalTable()
{
    global_u_ench_table.clear();
    global_p_ench_table.clear();
    global_groups.clear();
    table_list.clear();
    current_table = DataTableInfo();
}
