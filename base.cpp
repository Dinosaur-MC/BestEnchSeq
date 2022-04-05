#include "base.h"
#include "datamanager.h"

BASE::BASE(QObject *parent)
    : QObject{parent}
{
    qDebug() << "[BASE]";
}

//重载运算符
bool operator==(const Ench a, const Ench b) //比较魔咒
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

bool operator==(const Item a, const Item b) //比较物品
{
    if(a.name == b.name && a.durability == b.durability && a.penalty == b.penalty)
    {
        for(int i = 0; i < INIT_LENGTH; i++)
        {
            if(a.ench[i] != b.ench[i])
                return false;
        }
        return true;
    }
    return false;
}
bool operator!=(const Item a, const Item b)
{
    if(a.name != b.name || a.durability != b.durability || a.penalty != b.penalty)
        return true;

    for(int i = 0; i < INIT_LENGTH; i++)
    {
        if(a.ench[i] != b.ench[i])
            return true;
    }
    return false;
}

EnchTable* BASE::sTable(QString aim) //遍历魔咒池
{
    for(int i = 0; i < DM->ench_table_l; i++)
    {
        if(DM->ench_table[i].name == aim)
            return &DM->ench_table[i];
    }
    return NULL;
}

int BASE::sWeapon(QString aim, bool)    //遍历武器池
{
    for(int i = 0; i < DM->weapon_l; i++)
    {
        if(DM->weapon[i].name == aim)
            return i;
    }
    return -1;
}
Weapon* BASE::sWeapon(QString aim)    //遍历武器池
{
    for(int i = 0; i < DM->weapon_l; i++)
    {
        if(DM->weapon[i].name == aim)
            return &DM->weapon[i];
    }
    return NULL;
}

int BASE::sEnch(Ench *src, Ench aim, int len)    //在“src”中查找目标魔咒
{
    for(int i = 0; i < len; i++)
    {
        if(src[i] == aim)
            return i;
    }
    return -1;
}
int BASE::sEnch(Ench *src, QString aim, int len)    //在“src”中查找目标魔咒名
{
    for(int i = 0; i < len; i++)
    {
        if(src[i].name == aim)
            return i;
    }
    return -1;
}

QString BASE::IntToRoman(int num) //数字转罗马字符
{
    if(num > 10 || num < 1)
        return "level." + QString::number(num);
    else if(num == 10)
        return "X";
    else if(num == 9)
        return "IX";
    else if(num == 4)
        return "IV";
    QString str = "";
    if(num > 4)
    {
        str += "V";
        for(int i = num%5; i > 0; i--)
            str += "I";
    }
    else if(num < 4)
    {
        for(int i = num; i > 0; i--)
            str += "I";
    }
    return str;
    //Roman number needed 'N'!
}

int BASE::compareEnchs(Ench *e1, Ench *e2, int len) //比较魔咒列表
{
    int diff = 0;
    for(int i = 0; i < len && (e1[i].name != "" || e2[i].name != ""); i++)
    {
        if(e1[i].name != e2[i].name || e1[i].lvl != e2[i].lvl)
            diff++;
    }
    return diff;
}

