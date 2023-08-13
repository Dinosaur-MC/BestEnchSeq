#include "core.h"
#include <QHash>

DataTable current_table;
SettingsRuntime global_settings;

QString intToLevelText(int num)
{
    switch (num)
    {
    case 1:
        return QObject::tr("I");
        break;
    case 2:
        return QObject::tr("II");
        break;
    case 3:
        return QObject::tr("III");
        break;
    case 4:
        return QObject::tr("IV");
        break;
    case 5:
        return QObject::tr("V");
        break;
    case 6:
        return QObject::tr("VI");
        break;
    case 7:
        return QObject::tr("VII");
        break;
    case 8:
        return QObject::tr("VIII");
        break;
    case 9:
        return QObject::tr("IX");
        break;
    case 10:
        return QObject::tr("X");
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

QString ICMToString(ICM icm)
{
    switch (icm)
    {
    case ICM::Normal:
        return "Normal";
    case ICM::Poor:
        return "Poor";
    case ICM::Advanced:
        return "Advanced";
    default:
        return "Null";
    }
}
ICM StringToICM(QString str)
{
    if (str == "Normal")
        return ICM::Normal;
    else if (str == "Poor")
        return ICM::Poor;
    else if (str == "Advanced")
        return ICM::Advanced;
    else
        return ICM::Null;
}

Ench::Ench(const _Ench &e)
{
    if (e.id < CTE.size() && e.id >= 0)
    {
        this->name = CTE.at(e.id).name;
        this->mce = global_settings.edition;
        this->lvl = e.lvl;
        this->specials = e.specials;
    }
    else
    {
        this->name = "$Unknown";
        this->mce = MCE::Null;
        this->lvl = 0;
    }
}
bool Ench::isValid()
{
    if (this->name != "$Unknown" && this->lvl > 0 && this->mce != MCE::Null)
        return true;
    else
        return false;
}
bool operator==(const Ench &e1, const Ench &e2)
{
    if (e1.name == e2.name && e1.mce == e2.mce && e1.lvl == e2.lvl)
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
size_t qHash(const Ench &key, size_t seed)
{
    return qHash(key.name + QString::number((int)key.mce) + QString::number(key.lvl), seed);
}

_Ench::_Ench(const Ench &e)
{
    this->id = -1;
    this->lvl = 0;
    this->specials = e.specials;

    for (int i = 0; i < CTE.size(); i++)
    {
        if (CTE.at(i).name == e.name && CTE.at(i).editions.contains(e.mce))
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
size_t qHash(const _Ench &key, size_t seed)
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
    bool var = false;
    foreach (auto &e, e1.editions)
    {
        if (e2.editions.contains(e))
        {
            var = true;
            break;
        }
    }

    if (e1.name == e2.name && var)
        return true;
    else
        return false;
}
size_t qHash(const EnchData &key, size_t seed) // EnchData 仅以名称和版本(editions)区分
{
    return qHash(key.name, seed) ^ qHash(key.editions, seed << 2);
}

bool operator==(const Group &g1, const Group &g2)
{
    if (g1.name == g2.name)
        return true;
    else
        return false;
}
size_t qHash(const Group &key, size_t seed)
{
    return qHash(key.name, seed);
}

void DataTable::convertEnchTable() // 数据容器转换
{
    _enchs.clear();

    for (int i = 0; i < enchs.size(); i++) // Basis
    {
        _EnchData ed;
        ed.max_lvl = enchs.at(i).max_lvl;
        ed.poor_max_lvl = enchs.at(i).poor_max_lvl;
        ed.book_multiplier = enchs.at(i).book_multiplier;
        ed.item_multiplier = enchs.at(i).item_multiplier;
        ed.editions = enchs.at(i).editions;
        ed.specials = enchs.at(i).specials;
        _enchs.append(ed);
    }

    for (int i = 0; i < enchs.size(); i++)
    {
        for (auto &conf : enchs.at(i).conflictions) // Conflictions
        {
            int n = -1;
            QString name = conf;
            for (int j = 0; j < enchs.size(); j++)
            {
                if (enchs.at(j).name == name)
                {
                    n = j;
                    break;
                }
            }
            if (n >= 0)
            {
                _enchs[i].conflictions.insert(n);
            }
        }

        for (int j = 0; j < groups.size(); j++) // Group ID
        {
            if (groups.at(j).name == enchs.at(i).name)
                _enchs[i].group_id = j;
        }
    }
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

_Item::_Item(ItemType type, _EnchList enchs, int penalty, int durability)
{
    this->type = type;
    this->enchs = enchs;
    this->penalty = penalty;
    this->durability = durability;
}

Step::Step(const _Step &f)
{
    this->a = f.a;
    this->b = f.b;
    this->level_cost = f.level_cost;
    this->point_cost = f.point_cost;
    this->name_changing = f.name_changing;
}
_Step::_Step(const Step &f)
{
    this->a = f.a;
    this->b = f.b;
    this->level_cost = f.level_cost;
    this->point_cost = f.point_cost;
    this->name_changing = f.name_changing;
}

SettingsMap SettingsRuntime::toSettingsMap()
{
    SettingsMap smap;
    smap["version/file_version"] = file_version;
    smap["version/app_version"] = app_version;
    smap["default/edition"] = MCEToString(edition);
    smap["default/item_config"] = ICMToString(item_config);
    smap["default/algorithm"] = algorithm;
    smap["default/export_path"] = export_path;
    smap["default/language"] = language;
    smap["lever/auto_save"] = auto_save;
    smap["lever/auto_check_update"] = auto_check_update;
    smap["lever/enable_widely_reszie_window"] = enable_widely_reszie_window;
    smap["log/last_used_table"] = last_used_table;
    smap["log/last_edit"] = last_edit;
    return smap;
}
SettingsRuntime SettingsRuntime::fromSettingsMap(SettingsMap smap)
{
    SettingsRuntime settings;
    settings.file_version = smap["version/file_version"].toInt();
    settings.app_version = smap["version/app_version"].toInt();
    settings.edition = StringToMCE(smap["default/edition"].toString());
    settings.item_config = StringToICM(smap["default/item_config"].toString());
    settings.algorithm = smap["default/algorithm"].toString();
    settings.export_path = smap["default/export_path"].toString();
    settings.language = smap["default/language"].toString();
    settings.auto_save = smap["lever/auto_save"].toInt();
    settings.auto_check_update = smap["lever/auto_check_update"].toInt();
    settings.enable_widely_reszie_window = smap["lever/enable_widely_reszie_window"].toInt();
    settings.last_used_table = smap["log/last_used_table"].toString();
    settings.last_edit = smap["log/last_edit"].toDateTime();
    return settings;
}

SettingsRuntime defaultSettings()
{
    SettingsRuntime settings;
    settings.file_version = FILEVERSION;
    settings.app_version = VERSION_ID;
    settings.edition = MCE::Java;
    settings.item_config = ICM::Normal;
    settings.algorithm = "";
    settings.export_path = "./exports/";
    settings.language = "zh_cn";
    settings.auto_save = false;
    settings.auto_check_update = true;
    settings.enable_widely_reszie_window = false;
    settings.last_used_table = "";
    settings.last_edit = QDateTime();
    return settings;
}
