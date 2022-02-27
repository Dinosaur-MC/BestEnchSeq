#ifndef CALC_H
#define CALC_H

#include "common.h"
#include <QString>


class CALC
{
public:
    CALC();
    ~CALC();

private:
    int size = 10;
    Ench *p = new Ench[size]();

};

#endif // CALC_H
