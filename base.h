/*
 * 【基础库】
 * 宏
 * 结构体
 * 运算符重载
 * 基础函数
 * 基本数据
 */

#ifndef BASE_H
#define BASE_H

#include <QDebug>
#include <QObject>
#include <QIcon>

#define RESTART_CODE 19491001   //The restart code, a program restarting signal

#define PROGRAM_NAME_CN "最佳附魔顺序计算器"
#define PROGRAM_NAME_EN "The Best Enchanting Sequence"
#define VERSION "3.0_Beta"
#define VERSION_ID 12204120 //NyymmXXx; N:number of year, yy:the two last number of recent year, mm:recent month, XX:version number, x:small number
#define FILEVERSION 3   //Used to control the file version

#define AUTHOR "Dinosaur_MC(Dinosaur-MC)"
#define WEBSITE "https://github.com/Dinosaur-MC/BestEnchSeq"
#define STATEMENT "本程序为Github开源项目，完全永久免费使用！只推荐通过官方渠道获取本程序。\n任何对原程序进行收费售卖行为均为诈骗，谨防上当受骗！谨慎使用来路不明的非官方发布版本，谨防病毒入侵！"
#define STATEMENT_EN "This program is Github open source project, completely free to use forever! Access to this program is only recommended through official channels. \nAny selling of the original program for a fee is fraud, beware of being cheated! Be careful to use unofficial releases from unknown sources to guard against virus attacks!"

#define FILE_CONFIG "config.ini"
#define FILE_WEAPONTABLE "WeaponTable.csv"
#define FILE_ENCHTABLE "EnchantmentTable.csv"
#define TEXT_NOTE_SYMBOL '#'
#define UPDATE_JSON "https://raw.githubusercontent.com/Dinosaur-MC/BestEnchSeq/master/update.json"

#define INIT_LENGTH 64
#define ID_ECB "附魔书\nEnchanted Book"

//定义结构体
#define CONFIG_NUM 8
enum MCE {All=1, Java, Bedrock};  //枚举变量，MC编译版本
enum ICM {AllLevelEBook=1, BasicEBook, AdvanceMode};    //枚举变量，物品配置模式
enum ALGM {GlobalAverage=1, DifficultyFirst, Greedy, Enumeration, SimpleEnumeration};   //枚举变量，算法名
//enum ADDITION {None, IgnoreFixing, IgnorePenalty, IgnoreCostLimit};
struct Config {
  int config_version;   //配置文件版本，用于检查软件是否已更新
  MCE default_edition;  //默认MC编译版本
  ICM default_itemconfig;   //默认物品配置模式
  ALGM default_algorithm;    //默认算法
  QString export_path;  //默认流程导出路径
  bool constant;    //是否自动导出

  bool enableCustomEn;  //是否启用自定义魔咒
  bool enableCustomWe;  //是否启用自定义武器
  bool autoCheckUpdate; //是否自动检查更新
};

struct Weapon {
    QString name;   //武器名称
    QIcon icon; //武器图标
};

struct EnchTable {
    QString name;   //魔咒名称
    int emlvl;  //30级附魔台附魔一般最高可获得等级及宝藏魔咒（实际上用于BasicEBook物品配置中，是魔咒的最大可直接获得的魔咒等级，可自定义）
    int mlvl;   //魔咒最高等级
    int multiplier[2];  //魔咒乘数，[0]为物品乘数，[1]为附魔书乘数
    MCE edition;    //魔咒适用版本
    QString repulsion[INIT_LENGTH]; //冲突魔咒
    int suitable[INIT_LENGTH];  //适合的物品
};

struct Ench {
    QString name;   //魔咒名称
    int lvl;    //魔咒等级
};

struct EnchPlus {
    Ench e; //魔咒
    int emlvl;  //30级附魔台附魔一般最高可获得等级及宝藏魔咒
    int mlvl;   //魔咒最高等级
};

struct Item {
    QString name;   //物品名称
    Ench ench[INIT_LENGTH]; //物品魔咒
    int durability;   //物品耐久（附魔书应为0）
    int penalty;    //惩罚次数
};

struct Step {
    Item a; //目标物品
    Item b; //牺牲物品
    int cost;   //合并花费
    int penalty;    //惩罚次数
};


//重载运算符
bool operator==(const Ench a, const Ench b); //比较魔咒
bool operator!=(const Ench a, const Ench b);
bool operator==(const Item a, const Item b); //比较物品
bool operator!=(const Item a, const Item b);


//BASE类
class BASE : public QObject
{
    Q_OBJECT
public:
    explicit BASE(QObject *parent = nullptr);

    static EnchTable* sTable(QString aim); //遍历魔咒池
    static int sWeapon(QString aim, bool m);    //遍历武器池
    static Weapon* sWeapon(QString aim);
    static int sEnch(Ench *src, Ench aim, int len = INIT_LENGTH); //在“src”中查找目标魔咒
    static int sEnch(Ench *src, QString aim, int len = INIT_LENGTH);    //在“src”中查找目标魔咒名
    static QString IntToRoman(int num); //数字转罗马字符
    static int compareEnchs(Ench *e1, Ench *e2, int len = INIT_LENGTH); //比较魔咒列表

private:

signals:

};

#endif // BASE_H
