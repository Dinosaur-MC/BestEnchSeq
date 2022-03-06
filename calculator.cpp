#include "calculator.h"
using namespace std;


Calculator::Calculator(QObject *parent)
    : QObject{parent}
{
    if(Basic::mode[1] == 0)
    {
        qDebug() << "origin_ench_l & needed_ench_l" << Basic::origin_ench_l << Basic::needed_ench_l;
        A_list_l = Basic::origin_ench_l;
        A_list = new Item[A_list_l];
        for(int i = 0; i < A_list_l; i++)
        {
            A_list[i].ench[0].name = Basic::origin_ench[i].name;
            A_list[i].ench[0].lvl = Basic::origin_ench[i].lvl;
            A_list[i].duration = 0;
            A_list[i].penalty = 0;
            qDebug() << "A_list & origin_ench" << A_list[i].name << Basic::origin_ench[i].name;
        }

        B_list_l = Basic::needed_ench_l;
        B_list = new Item[B_list_l];
        for(int i = 0; i < B_list_l; i++)
        {
            B_list[i].ench[0].name = Basic::needed_ench[i].name;
            B_list[i].ench[0].lvl = Basic::needed_ench[i].lvl;
            B_list[i].duration = 0;
            B_list[i].penalty = 0;
        }
    }
    else if(Basic::mode[1] == 1)
    {

    }

    //Debug
//    qDebug() << "preForge:" << preForge(A_list[0], B_list[0], 0).cost;
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
    int penalty = max(A.penalty, B.penalty) + 1;

    int A_el = 0, B_el = 0;
    while(A_el < INIT_LENGTH && A.ench[A_el].name != "")
        A_el++;
    while(B_el < INIT_LENGTH && B.ench[B_el].name != "")
        B_el++;

    qDebug() << "preForge" << cost << penalty << A_el << A.ench[0].name << B_el << B.ench[0].name;

    if(Basic::mode[1] == 0)
    {
        if(Basic::mode[0] == 0)
        {
            for(int i = 0; i < B_el; i++)
            {
                int lever = 0;
                int p = Basic::searchTable(B.ench[i].name);
                for(int j = 0; j < Basic::ench_table[p].repulsion->count(); j++)
                {
                    if(Basic::searchEnch(A.ench, A_el, Basic::ench_table[p].repulsion[j]) != -1)
                    {
                        cost += 1;
                        lever++;
                        break;
                    }
                }

                if(lever == 0)
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * B.ench[i].lvl;
            }
        }
        else
        {
            for(int i = 0; i < B_el; i++)
            {
                int lever = 0;
                int p = Basic::searchTable(B.ench[i].name);
                for(int j = 0; j < Basic::ench_table[p].repulsion->count(); j++)
                {
                    if(Basic::searchEnch(A.ench, A_el, Basic::ench_table[p].repulsion[j]) != -1)
                    {
                        lever = 1;
                        break;
                    }
                }
                if(lever)
                    continue;

                int q = Basic::searchEnch(A.ench, A_el, B.ench[i].name);
                if(A.ench[q].lvl == Basic::ench_table[Basic::searchTable(A.ench[q].name)].mlvl)
                    continue;

                int lvl = B.ench[i].lvl;
                if( q != -1)
                {
                    if(A.ench[q].lvl == B.ench[i].lvl)
                    {
                        lvl = 1;
                    }
                    else if(A.ench[q].lvl < B.ench[i].lvl)
                    {
                        lvl -= A.ench[q].lvl;
                    }
                    else
                    {
                        lvl = 0;
                    }
                }

                cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * lvl;
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

