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
#define TEXT_PROGRAM_NAME QApplication::tr("最佳附魔顺序计算器")
#define TEXT_PROGRAM_NAME_EN "The Best Enchanting Sequence"
#define TEXT_PROGRAM_NAME_ABBR "BESQ"
#define VERSION_NAME "V4.2.0"
#define VERSION_ID 12308180 // NyymmXXx; N:number of century, yy:the two last number of recent year, mm:recent month, XX:capital number, x:sub number
#define FILEVERSION 5       // 文件版本

#define TEXT_AUTHOR "[Github] Dinosaur-MC"
#define LINK_HOME_PAGE "https://github.com/Dinosaur-MC/BestEnchSeq"
#define LINK_HELP_PAGE "https://www.bilibili.com/video/BV11T4y1D7c2"

// License & Announcement

#define LICENSE "GPL-3.0 License"
#define STATEMENT "本程序遵循 GPL-v3.0 协议，永久开源免费使用，不得商用！\n任何对原程序进行收费售卖行为均为诈骗，谨防上当受骗！\n谨慎使用来路不明的非官方发布版本，谨防病毒入侵！\n建议通过官方渠道获取本程序: \nhttps://github.com/Dinosaur-MC/BestEnchSeq"

// File Data

#define FILE_CONFIG "config.ini"
#define FILE_DEFAULT_TABLE ":/table/internal_table.json"
#define FILE_WEAPONTABLE "WeaponTable.csv"
#define FILE_ENCHTABLE "EnchantmentTable.csv"

#define PATH_TABLES "tables/"
#define PATH_EXPORTS "exports/"
#define PATH_ALGORITHMS "algorithms/"

// Spceial Sign

#define TEXT_SIGH_SYMBOL '$' // 特殊标识符
#define TEXT_NOTE_SYMBOL '#' // 注释符
#define FILEHEAD_CSV "\xef\xbb\xbf"

// Resources Links

#define LINK_UPDATE_DATA "https://raw.githubusercontent.com/Dinosaur-MC/BestEnchSeq/dev/update_data.json"

// Other

#define ICONPATH_ECB ":/icon/enchanted_book.png"
#define RESTART_CODE 19491001 // 重启代码

/* ----------------------------------------
 *       Headers & Global Functions
 * ---------------------------------------- */

// Headers

#include <QObject>
#include <QList>
#include <QSet>
#include <QMap>
#include <QIcon>
#include <QVariant>
#include <QDateTime>

// Functions

#define DEBUG(Class, Msg, Status) qDebug() << QString(QString("<") + Class + ">[" + Status + "]").toUtf8().data() << QString(Msg).toUtf8().data()

QString intToLevelText(int num);

/* ----------------------------------------
 *             Enum Variables
 * ---------------------------------------- */

enum class MCE
{
    Null,
    Java,
    Bedrock
}; // Minecraft Edition
QString MCEToString(MCE mce);
MCE StringToMCE(QString str);

enum class ICM
{
    Null,
    Normal,
    Poor,
    Advanced
}; // Item Configurating Mode
QString ICMToString(ICM icm);
ICM StringToICM(QString str);

enum class ItemType
{
    Book,
    Weapon,
    Stuff
}; // Item type

enum class ALGCFG
{
    Normal = 0x01,
    NoPlenalty = 0x02,
    NoRepairing = 0x04,
    IgnoreConflicting = 0x08,
    UnlimitedForgeCount = 0x10
}; // Algorithm configuration

/* ----------------------------------------
 *          Enchantment Container
 * ---------------------------------------- */

typedef QSet<QString> SpecialMethod;

struct _Ench;
struct Ench // User data
{
    QString name;
    MCE mce;
    int lvl;

    Ench() = default;
    Ench(QString name, MCE edition, int lvl) : name(name), mce(edition), lvl(lvl) {}
    Ench(const _Ench &e);
    bool isValid();
};
typedef QList<Ench> EnchList;
bool operator==(const Ench &e1, const Ench &e2);
bool operator<(const Ench &e1, const Ench &e2);
size_t qHash(const Ench &key, size_t seed = 0);

struct _Ench // Program data
{
    int id;
    int lvl;

    _Ench() = default;
    _Ench(int id, int lvl) : id(id), lvl(lvl) {}
    _Ench(const Ench &e);
    bool isValid();
};
typedef QList<_Ench> _EnchList;
bool operator==(const _Ench &e1, const _Ench &e2);
bool operator<(const _Ench &e1, const _Ench &e2);
size_t qHash(const _Ench &key, size_t seed = 0);

struct EnchData // User data
{
    QString name;
    int max_lvl;
    int poor_max_lvl;
    int book_multiplier;
    int item_multiplier;
    QSet<MCE> editions;
    SpecialMethod specials;
    QSet<QString> conflictions;
    QSet<QString> groups;

    bool isValid();
};
typedef QList<EnchData> EnchDataList;
bool operator==(const EnchData &e1, const EnchData &e2);
size_t qHash(const EnchData &key, size_t seed = 0);

struct _EnchData // Program data
{
    int max_lvl;
    int poor_max_lvl;
    int book_multiplier;
    int item_multiplier;
    QSet<MCE> editions;
    SpecialMethod specials;
    QSet<int> conflictions;
    int group_id;

    bool isValid();
};
typedef QList<_EnchData> _EnchDataList;

/* ----------------------------------------
 *             Group Container
 * ---------------------------------------- */

struct Group
{
    QString name;
    int max_durability;
    QString icon_path;
    EnchDataList enchantments;

    Group() = default;
    Group(QString group_name) { name = group_name; }
};
typedef QList<Group> GroupList;
bool operator==(const Group &g1, const Group &g2);
size_t qHash(const Group &key, size_t seed = 0);

struct _Group
{
    QString name;
    int max_durability;
    QList<int> enchantments;
};
typedef QList<_Group> _GroupList;

/* ----------------------------------------
 *             Table Container
 * ---------------------------------------- */

struct DataTableInfo
{
    QString file_name;
    QString description;
    QString table_version;
    bool enabled;
};

struct DataTable
{
    DataTableInfo info;
    EnchDataList enchs;
    GroupList groups;

    _EnchDataList _enchs;
    void convertEnchTable();
};
typedef QList<DataTable> DataTableList;
extern DataTable current_table;
#define CT current_table
#define CTI current_table.info
#define CTE current_table.enchs
#define CTG current_table.groups
#define CTEp current_table._enchs

/* ----------------------------------------
 *              Item Container
 * ---------------------------------------- */

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
 *            Result Container
 * ---------------------------------------- */

struct _Step;
struct Step // User data
{
    Item a;
    Item b;
    int level_cost;
    int point_cost;
    bool name_changing;

    Step() = default;
    Step(const _Step &f);
};
typedef QList<Step> StepList;

struct _Step // Program data
{
    _Item a;
    _Item b;
    int level_cost;
    int point_cost;
    bool name_changing;

    _Step() = default;
    _Step(const Step &f);
};
typedef QList<_Step> _StepList;

struct Summary
{
    MCE edition;
    ICM item_config;
    QString algorithm;
    ALGCFG alg_config;

    ItemPool input_items;
    ItemPool unused_items;
    Item output_item;

    int step_count;
    int calculate_times;
    uint max_level_cost;
    uint total_level_cost;
    ulong total_point_cost;

    bool processable;
};

struct Flow
{
    Summary summary;
    StepList steps;
};
typedef QList<Flow> FlowStack;

struct _Flow
{
    Summary summary;
    _StepList steps;
};
typedef QList<_Flow> _FlowStack;

/* ----------------------------------------
 *           Settings Container
 * ---------------------------------------- */

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
    bool enable_lax_window_resizing;
    bool deverloper_mode;

    // [log]
    QString last_used_table;
    QDateTime last_edit;

    // Convert functions
    SettingsMap toSettingsMap();
    static SettingsRuntime fromSettingsMap(SettingsMap smap);
};
extern SettingsRuntime global_settings;
SettingsRuntime defaultSettings();

#endif // CORE_H
