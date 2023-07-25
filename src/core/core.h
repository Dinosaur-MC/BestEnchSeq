#ifndef CORE_H
#define CORE_H

#define PROGRAM_NAME_CN "最佳附魔顺序计算器"
#define PROGRAM_NAME_EN "The Best Enchanting Sequence"
#define PROGRAM_NAME_ABBR "BESQ"
#define VERSION "4.1_Beta"
#define VERSION_ID 12301000 //NyymmXXx; N:number of century, yy:the two last number of recent year, mm:recent month, XX:version number, x:small number
#define FILEVERSION 4   // 文件版本


#define AUTHOR "Dinosaur_MC(Dinosaur-MC)"
#define WEBSITE "https://github.com/Dinosaur-MC/BestEnchSeq"
#define UPDATE_SITE "https://raw.githubusercontent.com/Dinosaur-MC/BestEnchSeq/master/update.json"
#define HELP_PAGE "https://www.bilibili.com/video/BV11T4y1D7c2"
#define STATEMENT "本程序为Github开源项目，完全永久免费使用！只推荐通过官方渠道获取本程序。\n任何对原程序进行收费售卖行为均为诈骗，谨防上当受骗！谨慎使用来路不明的非官方发布版本，谨防病毒入侵！"


#define FILEHEAD_CSV "\xef\xbb\xbf\x0d\x0a"
#define TEXT_NOTE_SYMBOL '#'
#define ID_ECB "$附魔书"
#define ICON_ECB QIcon(":/icon/enchanted_book.png")

#define DIR_EXPORT "./exports/"
#define FILE_CONFIG "config.ini"
#define FILE_WEAPONTABLE "WeaponTable.csv"
#define FILE_ENCHTABLE "EnchantmentTable.csv"

#define CONFIG_NUM 11 // 配置项数量

#define RESTART_CODE 19491001   // 重启代码


#include <QObject>
#include <QIcon>
#include <QVector>
#include <QDebug>
#include <QDateTime>


enum MCE {All=0, Java, Bedrock};  // 枚举变量，MC编译版本
enum ICM {AllLevelEBook=0, BasicEBook, AdvanceMode};    // 枚举变量，物品配置模式
enum ALGM {AlgM_0=0, AlgM_1, AlgM_2, AlgM_3};   // 枚举变量，算法名
enum PFADDN {Normal=0, NoRepair, NoRepulsion, NoPenalty};  // preforge 附加条件
enum ItemType {enchanted_book=0, weapon, consumable};   // 枚举变量，物品类型

// 配置信息
struct Config {
    int config_version;   // 配置文件版本，用于检查软件是否已更新
    MCE default_edition;  // 默认MC编译版本
    ICM default_itemconfig;   // 默认物品配置模式
    ALGM default_algorithm;    // 默认算法
    QString export_path;  // 默认流程导出路径
    bool auto_save;    // 是否自动保持结果

    bool enable_custom_en;  // 是否启用自定义魔咒
    bool enable_custom_we;  // 是否启用自定义武器
    bool auto_check_update; // 是否自动检查更新
    bool enable_reszie_window;  // 是否启用自由缩放

    bool deverloper_mode;  // 开发者模式
};

// 魔咒 {物品, 池}
struct Ench {
    QString name;   // 魔咒名称
    int lvl;    // 魔咒等级
};

struct EnchPlus {
    QString name;   // 魔咒名称
    MCE edition;    // 魔咒适用版本
    int max_level;  // 魔咒最高等级
    int poor_max_level; // 30级附魔台附魔一般最高可获得等级及宝藏魔咒（实际上用于BasicEBook物品配置中，是魔咒的最大可直接获得的魔咒等级，可自定义）
    int multiplier[2];  // 魔咒乘数，[0]为物品乘数，[1]为附魔书乘数
    QStringList repulsion; // 冲突的魔咒
};

// 武器 池
struct WeaponPlus {
    QString name;   // 武器名
    QIcon icon; // 武器图标
    QVector<EnchPlus> suitableE;    // 适配的魔咒
};

// 物品
struct Item {
    QString name;   // 物品名
    QIcon icon; // 物品图标
    ItemType type;   // 物品类型
    QVector<Ench> ench; // 物品魔咒
    int durability;   // 物品耐久
    int penalty;    // 惩罚数
};

// 流程
struct FlowStep {
    Item a; // 目标物品
    Item b; // 牺牲物品
    int levelCost;   // 等级花费
    int pointCost;   // 经验值花费
};

// 统计信息
struct Summary {
    MCE edition;
    ALGM algorithm;
    PFADDN pfaddn;
    Item input_item;
    Item output_item;
    int level_cost;
    long point_cost;
    int step_count;
    qint64 time_cost;
    bool processable;
};

// 加载内置数据
void loadInternalData(Config *config);
void loadInternalData(QVector<WeaponPlus> *wps);
void loadInternalData(QVector<EnchPlus> *eps);

// 等级转换
QString numToRoman(int);
long levelToPoint(int);


// 自定义Debug函数
enum MsgType {info=0, warning, wrong, error};
class MDebug : public QObject
{
    Q_OBJECT
public:
    explicit MDebug(QString name, QObject *parent = nullptr);
    QString name();
    void setName(QString name);

    void msg(MsgType type, QString str, bool l = false, int i = -1);
    void msg(QString str, bool l = false, int i = -1);
    void msg(MsgType type, QStringList strs, bool l = false, int i = -1);
    void msg(QStringList strs, bool l = false, int i = -1);

private:
    QString method_name;
};


// 数据分割
class StringKnife
{
public:
    explicit StringKnife();
    static QStringList lineSlice(QString str, char c);
    static QList<QStringList> crossSlice(QString str, char lc, char cc, int n = 2);
    static QList<QStringList> multicrossSlice(QString str,  char lc, char cc);

private:
    static MDebug mdb;
};


// 核心数据结构存储
#define CORE Core::getInstance()
class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
    static Core *getInstance();

    // 测试函数
    void _methodTest();
    void _methodTest_2();

    // Config
    Config config;
    MCE edition;
    ICM iten_config;
    ALGM algorithm;

    // Tables
    QVector<WeaponPlus> weapon_table;
    QVector<EnchPlus> ench_table;

    // Inputs
    Item origin;
    QVector<Ench> original_enchantments;
    QVector<Ench> needed_enchantments;
    QVector<Item> available_items;

    // Outputs
    Item product;
    Summary summary;
    QVector<FlowStep> flow;
    QVector<FlowStep> external_flow;

    // Other
    bool is_on_update;
    int enchantment_table_file_version;
    int weapon_table_file_version;
    QDateTime startDT;  // 程序启动时刻，用于程序日志等

private:
    MDebug mdb;

signals:

};


#endif // CORE_H
