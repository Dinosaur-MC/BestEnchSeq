#include "basicvariable.h"
#include "basicoperator.h"


void* CfgOpr::operator[](int i) {
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
    int *A = preforge(Item(), a, current_mce);
    int *B = preforge(Item(), b, current_mce);
    int r = A[0] - B[0];
    delete [] A;
    delete [] B;

    if(r < 0)
        return true;
    else
        return false;
}

bool operator>(const Item a, const Item b)
{
    int *A = preforge(Item(), a, current_mce);
    int *B = preforge(Item(), b, current_mce);
    int r = A[0] - B[0];
    delete [] A;
    delete [] B;

    if(r > 0)
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


FlowStep operator+(const Item a, const Item b)
{
    int *C = preforge(a, b, current_mce);  // 计算合并花费
    if(C == NULL)
        return FlowStep();

    int add;
    switch(pf_addtion) {
    case PFADDN::Normal:
        add = C[0];
        break;
    case PFADDN::NoRepair:
        add = C[1];
        break;
    case PFADDN::NoRepRepulsion:
        add = C[2];
        break;
    case PFADDN::Extreme:
        add = C[3];
        break;
    default:
        add = C[0];
        break;
    }
    delete [] C;    // 释放内存

    FlowStep fs;
    fs.a = a;
    fs.b = b;
    fs.levelCost = add;

    if(add <= 16)  // 计算经验值花费
        fs.pointCost = (add * add + 6 * add);
    else if(add <= 31)
        fs.pointCost = (2.5 * add * add - 40.5 * add + 360);
    else
        fs.pointCost = (4.5 * add * add - 162.5 * add + 2220);

    return fs;
}

