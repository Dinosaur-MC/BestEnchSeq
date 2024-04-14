#ifndef BASIC_H
#define BASIC_H

#include <QObject>
#include <QDebug>
#include <QIcon>

#define PROGRAM_NAME_CN "最佳附魔顺序计算器"
#define PROGRAM_NAME_EN "The Best Enchanting Sequence"
#define VERSION "2.1.0_Beta"
#define VERSION_ID 12404010 //NyymmXXx; N:1, yy:the two last number of recent year, mm:recent month, XX:version number, x:small number
#define AUTHOR "Dinosaur_MC(Dinosaur-MC)"
#define WEBSITE "https://github.com/Dinosaur-MC/BestEnchSeq"
#define UPDATE_JSON "https://raw.githubusercontent.com/Dinosaur-MC/BestEnchSeq/main/update.json"
#define STATEMENT "本程序为Github开源项目，完全免费使用！只推荐通过官方渠道获取本程序。\n任何对原程序进行收费售卖行为均为诈骗，谨防上当受骗！谨慎使用来路不明的非官方发布版本，谨防病毒入侵！"

#define FILE_CONFIG "config.ini"
#define FILE_WEAPONTABLE "WeaponTable.csv"
#define FILE_ENCHTABLE "EnchantmentTable.csv"
#define TEXT_NOTE_SYMBOL '#'

#define CONFIG_NUM 8
#define INIT_LENGTH 64
#define ID_ECB "附魔书\nEnchanted Book"

struct Config {
  int default_edition;
  int default_algorithm;
  QString export_path;
  bool constant;

  bool enableCustomEn;
  bool enableCustomWe;
  bool autoCheckUpdate;

  bool enableReszieWindow;
};

struct Weapon {
    QString name;
    QIcon icon;
};

struct EnchTable {
    QString name;
    int mlvl;
    int multiplier[2];
    int edition;
    QString repulsion[INIT_LENGTH];
    int suitable[INIT_LENGTH];
};

struct Ench {
    QString name;
    int lvl;
};

struct EnchPlus {
    Ench e;
    int mlvl;
};

struct Item {
    QString name;
    Ench ench[INIT_LENGTH];
    int duration;
    int penalty;
};

struct Step {
    Item a;
    Item b;
    int cost;
    int penalty;
};


class Basic : public QObject
{
    Q_OBJECT
public:
    explicit Basic(QObject *parent = nullptr);
    ~Basic();

    //Config
    static Config config;
    static int edition;
    static int mode[2];
    static bool lever[3];

    //Array
    static int ench_table_l;
    static int weapon_l;
    static int origin_ench_l;
    static int needed_ench_l;
    static int available_item_l;
    static int flow_list_l;
    static EnchTable *ench_table;
    static Weapon *weapon;
    static Ench *origin_ench;
    static Ench *needed_ench;
    static Item *available_item;
    static Step *flow_list;

    //Variable
    static Item OriginItem;
    static Item OutputItem;
    static int sumCost;

    //Public Method
    static int searchTable(QString aim);
    static int searchWeapon(QString aim);
    static int searchEnch(Ench *src, int len, QString aim);
    static int compareEnch(Ench *e1, Ench *e2, int len);
    static QString IntToRoman(int num);
    static int compareItem(Item A, Item B);

signals:

};

#endif // BASIC_H
