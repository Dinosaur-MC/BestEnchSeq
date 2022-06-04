#ifndef FLOWSTEP_H
#define FLOWSTEP_H

#include <QObject>
#include "base/Item.h"

struct Step {
    Item a; //目标物品
    Item b; //牺牲物品
    int cost;   //合并花费
    int penalty;    //惩罚次数
};

#endif // FLOWSTEP_H
