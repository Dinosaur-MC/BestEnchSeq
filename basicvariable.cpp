#include "basicvariable.h"
#include "basicoperator.h"


bool operator<(const Ench a, const Ench b)
{
    if(a.lvl < b.lvl)
        return true;
    else
        return false;
}

bool operator>(const Ench a, const Ench b)
{
    if(a.lvl > b.lvl)
        return true;
    else
        return false;
}


bool operator<(const Item a, const Item b)
{
    int cl_a = a.ench.count();
    int cl_b = b.ench.count();

    if(cl_a < cl_b)
        return true;
    else
        return false;
}

bool operator>(const Item a, const Item b)
{
    int cl_a = a.ench.count();
    int cl_b = b.ench.count();

    if(cl_a > cl_b)
        return true;
    else
        return false;
}

Item operator+=(const Item a, const Item b)
{
    Item tm;
    tm = forge(a, b);
    return tm;
}


FlowStep operator+(const Item a, const Item b)  // Quetion: MCE, Addtions
{
    FlowStep fs;
    int *C;
    C = preforge(a, b, MCE::Java);  // 计算合并花费

    fs.a = a;
    fs.b = b;
    fs.levelCost = C[0];

    if(C[0] <= 16)  // 计算经验值花费
        fs.pointCost = (C[0] * C[0] + 6 * C[0]);
    else if(C[0] <= 31)
        fs.pointCost = (2.5 * C[0] * C[0] - 40.5 * C[0] + 360);
    else
        fs.pointCost = (4.5 * C[0] * C[0] - 162.5 * C[0] + 2220);

    delete [] C;    // 释放内存

    return fs;
}

