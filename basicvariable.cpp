#include "basicvariable.h"
#include "basicoperator.h"


CfgOpr::CfgOpr(Config *c){
    cfg = c;
}

void CfgOpr::link(Config* c) {
    cfg = c;
}

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

