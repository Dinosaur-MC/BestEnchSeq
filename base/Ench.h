#ifndef ENCH_H
#define ENCH_H

#include <QObject>

struct Ench {
    QString name;   //魔咒名称
    int lvl;    //魔咒等级
};

bool operator==(const Ench a, const Ench b);
bool operator!=(const Ench a, const Ench b);

#endif // ENCH_H
