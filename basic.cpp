#include "basic.h"
using namespace std;

Config Basic::config;
int Basic::edition = 0;
int Basic::mode[2] = {0, 0};
bool Basic::lever[3] = {0, 0, 0};

int Basic::ench_table_l = INIT_LENGTH;
int Basic::weapon_l = INIT_LENGTH;
int Basic::origin_ench_l = INIT_LENGTH;
int Basic::needed_ench_l = INIT_LENGTH;
int Basic::available_item_l = INIT_LENGTH;
int Basic::flow_list_l = INIT_LENGTH;
EnchTable *Basic::ench_table = new EnchTable[Basic::ench_table_l];
Weapon *Basic::weapon = new Weapon[Basic::weapon_l];
Ench *Basic::origin_ench = new Ench[Basic::origin_ench_l];
Ench *Basic::needed_ench = new Ench[Basic::needed_ench_l];
Item *Basic::available_item = new Item[Basic::available_item_l];
Step *Basic::flow_list = new Step[Basic::flow_list_l];

Item Basic::OriginItem = {};
Item Basic::OutputItem = {};
int Basic::sumCost = 0;

Basic::Basic(QObject *parent)
    : QObject{parent}
{

}

int Basic::searchTable(QString aim)
{
    if(aim.isEmpty())
        return -1;
    for(int i = 0; i < Basic::ench_table_l; i++)
    {
        if(Basic::ench_table[i].name == aim)
        {
            return i;
        }
    }
    return -1;
}

int Basic::searchWeapon(QString aim)
{
    if(aim.isEmpty())
        return -1;
    for(int i = 0; i < Basic::weapon_l; i++)
    {
        if(Basic::weapon[i].name == aim)
        {
            return i;
        }
    }
    return -1;
}

int Basic::searchEnch(Ench *src, int len, QString aim)
{
    if(aim.isEmpty())
        return -1;
    for(int i = 0; i < len && src[i].name != ""; i++)
    {
        if(src[i].name == aim)
        {
            return i;
        }
    }
    return -1;
}

int Basic::compareEnch(Ench *e1, Ench *e2, int len)
{
    int diff = 0;
    for(int i = 0; i < len && (e1[i].name != "" || e2[i].name != ""); i++)
    {
        if(e1[i].name != e2[i].name || e1[i].lvl != e2[i].lvl)
            diff++;
    }
    return diff;
}

QString Basic::IntToRoman(int num)
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

int Basic::compareItem(Item A, Item B)
{
    int distance = 0;

    if(A.name != B.name)
        distance++;
    if(A.duration != B.duration)
        distance++;
    if(A.penalty != B.penalty)
        distance++;

    for(int i = 0; i < INIT_LENGTH; i++)
    {
        if(A.ench[i].name != B.ench[i].name || A.ench[i].lvl != B.ench[i].lvl)
            distance++;
    }

    return distance;
}

