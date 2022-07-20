#ifndef BASICVARIABLE_H
#define BASICVARIABLE_H


#include <QObject>
#include <QVector>
#include <QIcon>


enum ValueType {Null=0, Bool, Int, Float, Double, Char, String};  // 配置项数据类型
struct Option {
    QString name;   // 配置项键名
    ValueType value_type;   // 键值类型
};

QVector<Option> config_option;


enum MCE {All=0, Java, Bedrock};  // 枚举变量，MC编译版本
MCE current_mce;
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

Config current_config;

template<class T>   // 下标访问Config结构体 ~~脑回路清奇~~
class CfgOpr {
public:
    CfgOpr(Config* c = &current_config) {
        cfg = c;
    }
    void set(Config* c) {
        cfg = c;
    }

    T* operator[](int i) {
        switch (i) {
        case 0:
            return &cfg->config_version;
            break;
        case 1:
            return &cfg->default_edition;
            break;
        case 2:
            return &cfg->default_itemconfig;
            break;
        case 3:
            return &cfg->default_algorithm;
            break;
        case 4:
            return &cfg->export_path;
            break;
        case 5:
            return &cfg->constant;
            break;
        case 6:
            return &cfg->auto_check_update;
            break;
        case 7:
            return &cfg->enable_custom_we;
            break;
        case 8:
            return &cfg->enable_custom_en;
            break;
        case 9:
            return &cfg->enable_reszie_window;
            break;
        case 10:
            return &cfg->deverloper_mode;
            break;
        default:
            return NULL;
            break;
        }
    }

private:
    Config *cfg;
};


enum PFADDN {Normal=0, NoRepair, NoRepRepulsion, Extreme};  // preforge 附加条件
PFADDN pf_addtion;

struct Ench {
    int id; // 程序识别ID
    int lvl;    // 魔咒等级
};

bool operator<(const Ench, const Ench);
bool operator>(const Ench, const Ench);


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


QVector<raw_EnchPlus> raw_enchantment_table;
QVector<EnchPlus> enchantment_table;


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


QVector<raw_Weapon> raw_weapon_table;
QVector<Weapon> weapon_table;


struct Item {
    bool isECB;   // 是否为附魔书
    QVector<Ench> ench; // 物品魔咒
    int durability;   // 物品耐久
    int penalty;    // 惩罚数
};

bool operator<(const Item, const Item);
bool operator>(const Item, const Item);
Item operator+=(const Item, const Item);


struct FlowStep {
    Item a; // 目标物品
    Item b; // 牺牲物品
    int pointCost;   // 经验值花费
    int levelCost;   // 等级花费
};

FlowStep operator+(const Item, const Item);


//QVector<FlowStep> flow;


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
