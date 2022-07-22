#ifndef BASICVARIABLE_H
#define BASICVARIABLE_H


#include <QVector>
#include <QObject>
#include <QIcon>


enum ValueType {Null=0, Bool, Int, Float, Double, Char, String};  // 配置项数据类型
struct Option {
    QString name;   // 配置项键名
    ValueType value_type;   // 键值类型
};


enum MCE {All=0, Java, Bedrock};  // 枚举变量，MC编译版本
enum ICM {AllLevelEBook=0, BasicEBook, AdvanceMode};    // 枚举变量，物品配置模式
enum ALGM {GlobalAverage=0, DifficultyFirst, Greedy, Enumeration, SimpleEnumeration};   // 枚举变量，算法名

struct Config {
  int config_version;   // 配置文件版本，用于检查软件是否已更新

  MCE default_edition;  // 默认MC编译版本
  ICM default_itemconfig;   // 默认物品配置模式
  ALGM default_algorithm;    // 默认算法

  QString export_path;  // 默认流程导出路径
  bool constant;    // 是否自动导出
  bool auto_check_update; // 是否自动检查更新

  bool enable_custom_we;  // 是否启用自定义武器
  bool enable_custom_en;  // 是否启用自定义魔咒
  bool enable_reszie_window;  // 是否开启自由窗口大小模式

  bool deverloper_mode;  // 开发者模式
};


class CfgOpr    // 下标访问Config结构体 ~~脑回路清奇~~
{
public:
    CfgOpr(Config* c = nullptr);
    void link(Config* c);

    void* operator[](int i);

private:
    Config *cfg;
};


enum PFADDN {Normal=0, NoRepair, NoRepRepulsion, Extreme};  // preforge 附加条件


struct Ench {
    int id; // 程序识别ID
    int lvl;    // 魔咒等级
};

bool operator<(const Ench, const Ench);
bool operator>(const Ench, const Ench);


struct EnchPro {
    int id; // 程序识别ID
    QString text;   // 显示名称
    int minimum;    // 最小等级
    int maximum;    // 最大等级
};


// Raw Data
struct raw_EnchPlus {
    QString name;   // 魔咒名称
    MCE edition;    // 魔咒适用版本
    int max_level;  // 魔咒最高等级
    int poor_max_level; // 30级附魔台附魔一般最高可获得等级及宝藏魔咒（实际上用于BasicEBook物品配置中，是魔咒的最大可直接获得的魔咒等级，可自定义）
    int multiplier[2];  // 魔咒乘数，[0]为物品乘数，[1]为附魔书乘数
    QVector<QString> repulsion; // 冲突的魔咒
    QVector<bool> suitable;  // 适配的物品
};

// Optimized Data
struct EnchPlus {
    int id; // 程序识别ID
    MCE edition;    // 魔咒适用版本
    int max_level;  // 魔咒最高等级
    int poor_max_level; // 30级附魔台附魔一般最高可获得等级及宝藏魔咒（实际上用于BasicEBook物品配置中，是魔咒的最大可直接获得的魔咒等级，可自定义）
    int multiplier[2];  // 魔咒乘数，[0]为物品乘数，[1]为附魔书乘数
    QVector<int> repulsion; // 冲突的魔咒
};


// Raw Data
struct raw_Weapon {
    QString name;   // Weapon名
    QIcon icon; // Weapon图标
};

// Optimized Data
struct Weapon {
    int id; // Weapon ID
    QVector<EnchPlus> suitableE;    // 适配的物品
};


struct Item {
    int type;   // 物品种类，-1 时为附魔书
    QVector<Ench> ench; // 物品魔咒
    int durability;   // 物品耐久
    int penalty;    // 惩罚数
};

struct ItemPro {
    QIcon icon; // 物品图标
    QString name;   // 物品名称
    QString ench_set;   // 魔咒信息
};


struct FlowStep {
    Item a; // 目标物品
    Item b; // 牺牲物品
    int levelCost;   // 等级花费
};

struct FlowStepPro {
    ItemPro a; // 目标物品
    ItemPro b; // 牺牲物品
    int levelCost;   // 等级花费
    int pointCost;   // 经验值花费
};


struct Summary {
    MCE edition;
    ALGM algorithm;
    raw_Weapon input_item;
    raw_Weapon output_item;
    QVector<Ench> input_item_ench;
    QVector<Ench> output_item_ench;
    int level_cost;
    int point_cost;
    int step_count;
    int time_cost;
    bool processable;
};

#endif // BASICVARIABLE_H
