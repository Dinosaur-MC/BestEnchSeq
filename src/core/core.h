#ifndef CORE_H
#define CORE_H

/* ------------------------------------------------------------
 *
 *                       BestEnchSeq Core
 *
 * ------------------------------------------------------------ */

#include "built-in_data.h"
#include <QObject>
#include <QVector>
#include <QSet>
#include <QMap>
#include <QIcon>
#include <QVariant>
#include <QDateTime>

#define DEBUG(Class, Msg, Status) qDebug() << QString(QString("<") + Class + ">[" + Status + "]").toUtf8().data() << QString(Msg).toUtf8().data()

QString intToRoman(int num);

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
struct _Ench;
struct Ench // User data
{
    QString name;
    QSet<MCE> mce;
    int lvl;

    Ench() = default;
    Ench(const _Ench &e);
    bool isValid();
};
bool operator==(const Ench &e1, const Ench &e2);
bool operator<(const Ench &e1, const Ench &e2);
uint qHash(const Ench &key, uint seed = 0);

struct _Ench // Program data
{
    int id;
    int lvl;

    _Ench() = default;
    _Ench(const Ench &e);
    bool isValid();
};
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
    QSet<QString> conflictions;
    QSet<QString> groups;

    bool isValid();
};
bool operator==(const EnchData &e1, const EnchData &e2);
uint qHash(const EnchData &key, uint seed = 0);
extern QVector<EnchData> global_u_ench_table;

struct _EnchData // Program data
{
    int max_lvl;
    int poor_max_lvl;
    int book_multiplier;
    int item_multiplier;
    QSet<MCE> editions;
    QSet<int> conflictions;

    bool isValid();
};
extern QVector<_EnchData> global_p_ench_table;
QVector<_EnchData> convertEnchTable(QVector<EnchData> &table);

/* ----------------------------------------
 *             Group Container
 * ---------------------------------------- */

struct Group
{
    QString name;
    QString icon_path;
    QSet<EnchData> enchantments;
};
bool operator==(const Group &g1, const Group &g2);
uint qHash(const Group &key, uint seed = 0);
extern QSet<Group> global_groups;

/* ----------------------------------------
 *              Item Container
 * ---------------------------------------- */

enum class ItemType
{
    Book,
    Weapon,
    Stuff
};

struct Item // User data
{
    ItemType type;
    QIcon icon;
    QSet<Ench> enchs;
    int penalty_lvl;
    int durability;
};

struct _Item // Program data
{
    ItemType type;
    QSet<_Ench> enchs;
    int penalty_lvl;
    int durability;
};

/* ----------------------------------------
 *            FlowStep Container
 * ---------------------------------------- */

struct FlowStep // User data
{
    Item a;
    Item b;
    int level_cost;
    int point_cost;
    bool name_changing;
};

struct _FlowStep // Program data
{
    _Item a;
    _Item b;
    int level_cost;
    int point_cost;
};

/* ----------------------------------------
 *         Configuration Container
 * ---------------------------------------- */

extern QMap<QString, QVariant> global_settings;
void registerSettings();

/* ----------------------------------------
 *             Other Container
 * ---------------------------------------- */

enum class ICM
{
    Normal,
    Poor,
    Advanced
}; // Item Configurating Mode
enum class ALG
{
    Intelligent,
    Enumerating,
    Manual
}; // Algorithm
struct Summary
{
    MCE edition;
    ICM item_config;
    ALG algorithm;
    int step_count;
    int level_cost;
    long point_cost;
    qint64 time_cost;
    QVector<Item> input_items;
    QVector<Item> unused_items;
    Item output_item;
    QVector<int> calculate_times;
    bool processable;
};

struct DataTableInfo
{
    QString name;
    QString description;
    QString table_version;
};
extern QVector<DataTableInfo> table_list;
extern DataTableInfo current_table;

void initializeGlobalTable();

#endif // CORE_H
