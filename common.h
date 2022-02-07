#ifndef COMMON_H
#define COMMON_H

#include<QMainWindow>

struct EnchTable {
    QString name;
    int max_lvl;
    int multiplier[2];
    bool suitable[13];
    QStringList repulsion;
};
//bool suitable:
//    0 for_sword;
//    1 for_pickaxe;
//    2 for_axe;
//    3 for_shovel;
//    4 for_hoe;
//    5 for_helmet;
//    6 for_chestplate;
//    7 for_leggings;
//    8 for_boots;
//    9 for_bow;
//    10 for_crossbow;
//    11 for_trident;
//    12 for_fishing_rod;

struct Ench {
    QString name;
    int lvl;
};

struct Item {
    QString type;
    int Durability;
    int Penalty;
    Ench ench[64];
};

struct Step {
    Item tar;
    Item sac;
    int penalty;
    int cost;
};

class Common : public QObject
{
    Q_OBJECT

public:
    Common(QObject *parent = nullptr);



    static int searchTable(QString aim);
    static int searchEach(Ench *src, int len, QString aim);
    static int min(Item *item, int len);

    static EnchTable Table[64];
    static Ench Existed[64];
    static Item InputItem;
    static Item OutputItem;


    static int processing_mode;
    static Step Flow[64];

private:

signals:

};

#endif // COMMON_H
