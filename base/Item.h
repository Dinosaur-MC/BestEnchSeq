#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include "base/EnchSet.h"

#define ID_ECB "附魔书\nEnchanted Book"

class Item : public QObject
{
    Q_OBJECT

public:
    explicit Item();
    ~Item();

    QString name;   //物品名称
    EnchSet *ench; //物品魔咒
    int durability;   //物品耐久（附魔书应为0）
    int penalty;    //惩罚次数

    void copy(Item *it);
    int compare(Item *it);
    void drop();

    enum ForgeMode {};
    Item *forge(Item it);
    int forge(Item it, int m);

};


#endif // ITEM_H
