#include "common.h"
#include <QDebug>
using namespace std;

EnchTable Common::Table[64];
Ench Common::Existed[64];
Item Common::InputItem;
Item Common::OutputItem;

bool Common::ignore_Fixing = 0;
bool Common::ignore_CL = 0;
bool Common::ignore_PWP = 0;
int Common::processing_mode = 0;
Step Common::Flow[64];

Common::Common(QObject *parent) :
    QObject(parent)
{

}

int Common::searchTable(QString aim)
{
    for(int i = 0; &Common::Table[i] != end(Common::Table); i++)
    {
        if(Common::Table[i].name == aim)
        {
            return i;
        }
    }
    return -1;
}

int Common::searchEach(Ench *src, int len, QString aim)
{
    for(int i = 0; i < len && src[i].name != ""; i++)
    {
        if(src[i].name == aim)
        {
            return i;
        }
    }
    return -1;
}

int Common::compareEnch(Ench *e1, Ench *e2, int len)
{
    int diff = 0;
    for(int i = 0; i < len && (e1[i].name != "" || e2[i].name != ""); i++)
    {
        if(e1[i].name != e2[i].name || e1[i].lvl != e2[i].lvl)
            diff++;
    }
    return diff;
}

QString Common::IntToRoman(int num)
{
    if(num > 10 || num < 1)
        return QString::number(num);
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

