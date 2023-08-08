#ifndef CORE_H
#define CORE_H

/* ------------------------------------------------------------
 *
 *                      BestEnchSeq Core
 *
 * ------------------------------------------------------------ */

/* ----------------------------------------
 *              Built-in Data
 * ---------------------------------------- */

// Application Infomation

#define TEXT_PROJECT_NAME "BestEnchSeq"
#define TEXT_PROGRAM_NAME_CN "最佳附魔顺序计算器"
#define TEXT_PROGRAM_NAME_EN "The Best Enchanting Sequence"
#define TEXT_PROGRAM_NAME_ABBR "BESQ"
#define VERSION_NAME "v4.2.0"
#define VERSION_ID 12307020 // NyymmXXx; N:number of century, yy:the two last number of recent year, mm:recent month, XX:capital number, x:sub number

#define TEXT_AUTHOR "Dinosaur_MC(Dinosaur-MC)"
#define LINK_HOME_PAGE "https://github.com/Dinosaur-MC/BestEnchSeq"
#define LINK_HELP_PAGE "https://www.bilibili.com/video/BV11T4y1D7c2"

// License & Announcement

#define LICENSE "GPL-v3.0"
#define STATEMENT "本程序为Github开源项目，完全永久免费使用！只推荐通过官方渠道获取本程序。\n任何对原程序进行收费售卖行为均为诈骗，谨防上当受骗！谨慎使用来路不明的非官方发布版本，谨防病毒入侵！"

// File Data

#define FILE_CONFIG "config.ini"
#define FILE_WEAPONTABLE "WeaponTable.csv"
#define FILE_ENCHTABLE "EnchantmentTable.csv"

#define FILEVERSION 5 // 文件版本

#define TEXT_SIGH_SYMBOL '@' // 特殊标识符
#define TEXT_NOTE_SYMBOL '#' // 注释符
#define FILEHEAD_CSV "\xef\xbb\xbf"

// Resources Links

#define LINK_UPDATE_DATA "https://raw.githubusercontent.com/Dinosaur-MC/BestEnchSeq/master/update_data.json"

// Other

#define ICON_ECB QIcon(":/icon/enchanted_book.png")
#define RESTART_CODE 19491001 // 重启代码

/* ----------------------------------------
 *       Headers & Global Functions
 * ---------------------------------------- */

#include <QObject>
#include <QList>
#include <QSet>
#include <QMap>
#include <QIcon>
#include <QVariant>
#include <QDateTime>

#define DEBUG(Class, Msg, Status) qDebug() << QString(QString("<") + Class + ">[" + Status + "]").toUtf8().data() << QString(Msg).toUtf8().data()

QString intToLevelText(int num);

/* ----------------------------------------
 *          Enchantment Container
 * ---------------------------------------- */

enum class MCE
{
    Null,
    Java,
    Bedrock
}; // Minecraft Edition
QString MCEToString(MCE mce);
MCE StringToMCE(QString str);

enum class SpecialMethod
{
    Null,
    OnceEffect,
    PenalyErase,
    Repairing
}; // Special method of enchantment
QString SpecialMethodToString(SpecialMethod spm);
SpecialMethod StringToSpecialMethod(QString str);

struct _Ench;
struct Ench // User data
{
    QString name;
    MCE mce;
    int lvl;
    QSet<SpecialMethod> specials;

    Ench() = default;
    Ench(QString name, int lvl) : name(name), lvl(lvl) {}
    Ench(const _Ench &e);
    bool isValid();
};
typedef QList<Ench> EnchList;
bool operator==(const Ench &e1, const Ench &e2);
bool operator<(const Ench &e1, const Ench &e2);
uint qHash(const Ench &key, uint seed = 0);

struct _Ench // Program data
{
    int id;
    int lvl;
    QSet<SpecialMethod> specials;

    _Ench() = default;
    _Ench(int id, int lvl) : id(id), lvl(lvl) {}
    _Ench(const Ench &e);
    bool isValid();
};
typedef QList<_Ench> _EnchList;
bool operator==(const _Ench &e1, const _Ench &e2);
bool operator<(const _Ench &e1, const _Ench &e2);
uint qHash(const _Ench &key, uint seed = 0);

struct EnchData // User data
{
    QString name;
    int max_lvl;
    int poor_max_lvl;
    int book_multiplier;
    int item_multiplier;
    QSet<MCE> editions;
    QSet<SpecialMethod> specials;
    QSet<QString> conflictions;
    QSet<QString> groups;

    bool isValid();
};
typedef QList<EnchData> EnchDataList;
bool operator==(const EnchData &e1, const EnchData &e2);
uint qHash(const EnchData &key, uint seed = 0);
extern EnchDataList global_u_ench_table;

struct _EnchData // Program data
{
    int max_lvl;
    int poor_max_lvl;
    int book_multiplier;
    int item_multiplier;
    QSet<MCE> editions;
    QSet<SpecialMethod> specials;
    QSet<int> conflictions;
    int group_id;

    bool isValid();
};
typedef QList<_EnchData> _EnchDataList;
extern _EnchDataList global_p_ench_table;

_EnchDataList convertEnchTable(EnchDataList &table);

/* ----------------------------------------
 *             Group Container
 * ---------------------------------------- */

struct Group
{
    QString name;
    int max_durability;
    QString icon_path;
    QSet<EnchData> enchantments;
};
typedef QList<Group> GroupList;
bool operator==(const Group &g1, const Group &g2);
uint qHash(const Group &key, uint seed = 0);
extern GroupList global_groups;

/* ----------------------------------------
 *              Item Container
 * ---------------------------------------- */

enum class ItemType
{
    Book,
    Weapon,
    Stuff
};

struct _Item;
struct Item // User data
{
    ItemType type;
    QIcon icon;
    QList<Ench> enchs;
    int penalty;
    int durability;

    Item() = default;
    Item(const _Item &it);
};
typedef QList<Item> ItemPool;

struct _Item // Program data
{
    ItemType type;
    _EnchList enchs;
    int penalty;
    int durability;

    _Item() = default;
    _Item(ItemType type, _EnchList enchs, int penalty, int durability);
    _Item(const Item &it);
};
typedef QList<_Item> _ItemPool;

/* ----------------------------------------
 *            FlowStep Container
 * ---------------------------------------- */

struct _FlowStep;
struct FlowStep // User data
{
    Item a;
    Item b;
    int level_cost;
    int point_cost;
    bool name_changing;

    FlowStep() = default;
    FlowStep(const _FlowStep &f);
};
typedef QList<FlowStep> FlowList;
typedef QList<FlowList> FlowStack;

struct _FlowStep // Program data
{
    _Item a;
    _Item b;
    int level_cost;
    int point_cost;
    bool name_changing;

    _FlowStep() = default;
    _FlowStep(const FlowStep &f);
};
typedef QList<QList<_FlowStep>> _FlowStack;

/* ----------------------------------------
 *         Configuration Container
 * ---------------------------------------- */

enum class ICM
{
    Null,
    Normal,
    Poor,
    Advanced
}; // Item Configurating Mode
QString ICMToString(ICM icm);
ICM StringToICM(QString str);

typedef QMap<QString, QVariant> SettingsMap;
struct SettingsRuntime
{
    // [version]
    int file_version;
    int app_version;

    // [default]
    MCE edition;
    ICM item_config;
    QString algorithm;
    QString export_path;
    QString language;

    // [lever]
    bool auto_save;
    bool auto_check_update;
    bool enable_widely_reszie_window;
    bool deverloper_mode;

    // [log]
    QString last_used_table;
    QDateTime last_edit;

    // Convert functions
    SettingsMap toSettingsMap();
    void fromSettingsMap(SettingsMap smap);
};
extern SettingsRuntime global_settings;

// void registerSettings();

/* ----------------------------------------
 *             Other Container
 * ---------------------------------------- */

enum class ALGCFG
{
    Normal = 0x01,
    NoPlenalty = 0x02,
    NoRepairing = 0x04,
    EnableConflicting = 0x08,
    UnlimitedForgeCount = 0x10
};

struct Summary
{
    MCE edition;
    ICM item_config;
    //    ALG algorithm;
    QString algorithm;
    ALGCFG alg_config;

    QList<Item> input_items;
    QList<Item> unused_items;
    Item output_item;

    int step_count;
    uint max_level_cost;
    uint total_level_cost;
    ulong total_point_cost;
    QList<int> calculate_times;

    bool processable;
};

struct DataTableInfo
{
    QString name;
    QString description;
    QString table_version;
};
extern QList<DataTableInfo> table_list;
extern DataTableInfo current_table;

void initializeGlobalTable();

#endif // CORE_H
