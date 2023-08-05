#include "core.h"

QList<EnchData> global_u_ench_table;
QList<_EnchData> global_p_ench_table;
QList<Group> global_groups;
QMap<QString, QVariant> global_settings;
QList<DataTableInfo> table_list;
DataTableInfo current_table;

QString intToLevelText(int num)
{
    switch (num)
    {
    case 1:
        return QObject::tr("I");
        break;
    case 2:
        return QObject::tr("I");
        break;
    case 3:
        return QObject::tr("I");
        break;
    case 4:
        return QObject::tr("I");
        break;
    case 5:
        return QObject::tr("I");
        break;
    case 6:
        return QObject::tr("I");
        break;
    case 7:
        return QObject::tr("I");
        break;
    case 8:
        return QObject::tr("I");
        break;
    case 9:
        return QObject::tr("I");
        break;
    case 10:
        return QObject::tr("I");
        break;
    default:
        return QObject::tr("level.") + QString::number(num);
        break;
    }
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

QString SpecialMethodToString(SpecialMethod spm)
{
    switch (spm)
    {
    case SpecialMethod::OnceEffect:
        return "OnceEffect";
    case SpecialMethod::PenalyErase:
        return "PenalyErase";
    case SpecialMethod::Repairing:
        return "Repairing";
    default:
        return "Null";
    }
}
SpecialMethod StringToSpecialMethod(QString str)
{
    if (str == "OnceEffect")
        return SpecialMethod::OnceEffect;
    else if (str == "PenalyErase")
        return SpecialMethod::PenalyErase;
    else if (str == "Repairing")
        return SpecialMethod::Repairing;
    else
        return SpecialMethod::Null;
}

Ench::Ench(const _Ench &e)
{
    if (e.id < global_u_ench_table.size() && e.id >= 0)
    {
        this->name = global_u_ench_table.at(e.id).name;
        this->mce = global_u_ench_table.at(e.id).editions;
        this->lvl = e.lvl;
        this->specials = e.specials;
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
    this->specials = e.specials;

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

QList<_EnchData> convertEnchTable(QList<EnchData> &table) // 数据容器转换
{
    QList<_EnchData> _table;

    for (int i = 0; i < table.size(); i++) // Basis
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
        for (auto &conf : table.at(i).conflictions) // Conflictions
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

        for (int j = 0; j < global_groups.size(); j++) // Group ID
        {
            if (global_groups.at(j).name == table.at(i).name)
                _table[i].group_id = j;
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

Item::Item(const _Item &it)
{
    this->type = it.type;
    this->penalty = it.penalty;
    this->durability = it.durability;
    foreach (auto &e, it.enchs)
        this->enchs.append(e);
}
_Item::_Item(const Item &it)
{
    this->type = it.type;
    this->penalty = it.penalty;
    this->durability = it.durability;
    foreach (auto &e, it.enchs)
        this->enchs.append(e);
}

FlowStep::FlowStep(const _FlowStep &f)
{
    this->a = f.a;
    this->b = f.b;
    this->level_cost = f.level_cost;
    this->point_cost = f.point_cost;
    this->name_changing = f.name_changing;
}
_FlowStep::_FlowStep(const FlowStep &f)
{
    this->a = f.a;
    this->b = f.b;
    this->level_cost = f.level_cost;
    this->point_cost = f.point_cost;
    this->name_changing = f.name_changing;
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
