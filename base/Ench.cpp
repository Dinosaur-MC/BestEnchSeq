#include "base/Ench.h"

bool operator==(const Ench a, const Ench b)
{
    if(a.name == b.name && a.lvl == b.lvl)
        return true;
    return false;
}

bool operator!=(const Ench a, const Ench b)
{
    if(a.name != b.name || a.lvl != b.lvl)
        return true;
    return false;
}
