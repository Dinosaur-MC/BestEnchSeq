#include "calculator.h"
using namespace std;


Calculator::Calculator(QObject *parent)
    : QObject{parent}
{
    if(Basic::mode[1] == 0)
    {
        A_list_l = Basic::origin_ench_l;
        A_list = new Item[A_list_l];
        for(int i = 0; i < A_list_l; i++)
        {
            A_list[i].name = Basic::origin_ench[i].name;
            A_list[i].ench[0].lvl = Basic::origin_ench[i].lvl;
        }

        B_list_l = Basic::needed_ench_l;
        B_list = new Item[B_list_l];
        for(int i = 0; i < B_list_l; i++)
        {
            B_list[i].name = Basic::needed_ench[i].name;
            B_list[i].ench[0].lvl = Basic::needed_ench[i].lvl;
        }
    }
    else if(Basic::mode[1] == 1)
    {

    }
}


void Calculator::Alg_DifficultFirst()
{

}

void Calculator::Alg_Greedy()
{

}

void Calculator::Alg_Enumeration()
{

}

Step Calculator::preForge(Item A, Item B, int mode)
    //mode; 0:Normal, 1:Ignore pennalty, 2:Ignore duration, 3:Ignore both
{
    Step s;
    int cost = 0;
    int penalty = max(A.penalty, B.penalty);

    if(Basic::mode[1] == 0)
    {
        if(Basic::mode[0] == 0)
        {
            for(int i = 0; i < INIT_LENGTH && B.ench[i].name != ""; i++)
            {
                cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * B.ench[i].lvl;
            }
        }
        else
        {
            for(int i = 0; i < INIT_LENGTH && B.ench[i].name != ""; i++)
            {
                int p = Basic::searchEnch(Basic::origin_ench, Basic::origin_ench_l, B.ench[i].name);
                if( p != -1)
                {
                    int lvl = Basic::origin_ench[p].lvl;;
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * lvl;
                }
            }
        }
    }
    else if(Basic::mode[1] == 1)
    {

    }

    if(mode == 0 || mode == 2)
    {
        cost += pow(2, A.penalty) - 1;
        cost += pow(2, B.penalty) - 1;
    }
    if((mode == 0 || mode == 1) && A.duration != 100 && B.duration != 0)
        cost += 2;

    s.cost = cost;
    s.penalty = penalty;
    return s;
}

Step Calculator::Forge(Item A, Item B, int mode)
{

}

void Calculator::sort()
{

}

