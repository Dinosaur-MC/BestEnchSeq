#include "calculator.h"
using namespace std;


Calculator::Calculator(QObject *parent)
    : QObject{parent}
{
    qDebug() << "[Calculator]";

    A_item = Basic::OriginItem;
    A_item_el = 0;
    while(A_item_el < INIT_LENGTH && Basic::origin_ench[A_item_el].name != "")
        A_item_el++;
    for(int i = 0; i < A_item_el; i++)
    {
        A_item.ench[i] = Basic::origin_ench[i];
    }

    if(Basic::mode[1] == 0)
        //Basic::mode; 0:basic mode(enchanted book only), 1:advance mode
    {   
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


    if(!Basic::lever[0] && !Basic::lever[1])
        mode = 0;
    else if(Basic::lever[0] && !Basic::lever[1])
        mode = 1;
    else if(!Basic::lever[0] && Basic::lever[1])
        mode = 2;
    else
        mode = 3;

    cost_list_l = B_list_l;
    cost_list = new COSTL[cost_list_l];
    for(int i = 0; i < cost_list_l; i++)
    {
        Step s = preForge(Item({}), B_list[i], mode);
        cost_list[i] = {i, s.cost, s.penalty};
    }
    qDebug() << "Calculator init finshed";
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
    s.a = A;
    s.b = B;
    int cost = 0;
    int penalty = max(A.penalty, B.penalty) + 1;

    int A_el = 0, B_el = 0;
    while(A_el < INIT_LENGTH && A.ench[A_el].name != "")
        A_el++;
    while(B_el < INIT_LENGTH && B.ench[B_el].name != "")
        B_el++;

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
                if(lever)
                    continue;

                if(B.name == "Enchante Book" || B.duration == 0)
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * B.ench[i].lvl;
                else
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[0] * B.ench[i].lvl;
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

                if(B.name == "Enchante Book" || B.duration == 0)
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * lvl;
                else
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[0] * lvl;
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
    qDebug() << "preForge:" << cost << penalty << A_el << A.ench[0].name << A.ench[0].lvl << B_el << B.ench[0].name << B.ench[0].lvl;
    return s;
}

Item Calculator::Forge(Item A, Item B)
{
    int A_el = 0, B_el = 0;
    while(A_el < INIT_LENGTH && A.ench[A_el].name != "")
        A_el++;
    while(B_el < INIT_LENGTH && B.ench[B_el].name != "")
        B_el++;

    for(int i = 0; i < INIT_LENGTH && i < B_el; i++)
    {
        int lever = 0;
        int p = Basic::searchTable(B.ench[i].name);
        for(int j = 0; j < Basic::ench_table[p].repulsion->count(); j++)
        {
            if(Basic::searchEnch(A.ench, A_el, Basic::ench_table[p].repulsion[j]) != -1)
            {
                lever++;
                break;
            }
        }
        if(lever)
            continue;

        int q = Basic::searchEnch(A.ench, A_el, B.ench[i].name);
        if(q != -1)
        {
            if(A.ench[q].lvl == B.ench[i].lvl && A.ench[q].lvl != Basic::ench_table[Basic::searchTable(A.ench[q].name)].mlvl)
            {
               A.ench[q].lvl++;
            }
            else if(A.ench[q].lvl < B.ench[i].lvl)
            {
                A.ench[q].lvl = B.ench[i].lvl;
            }
        }
    }

    if(A.duration != 100 && B.duration != 0)
    {
        A.duration += B.penalty;
        A.duration *= 1.12;
        if(A.duration > 100)
            A.duration = 100;
    }
    A.penalty = max(A.penalty, B.penalty) + 1;

    return A;
}

void Calculator::sort()
{

}

