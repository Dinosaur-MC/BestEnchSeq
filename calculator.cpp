#include "calculator.h"

using namespace std;


Calculator::Calculator(QObject *parent)
    : QObject{parent}
{
    qDebug() << "[Calculator]";
    //Basic::mode[1]; 0:basic mode(enchanted book only), 1:advance mode

    pool.append(Basic::OriginItem);
    if(Basic::mode[1] == 0)
    {
        for(int i = 0; i < Basic::needed_ench_l; i++)
        {
            Item tm = {"Enchanted Book", {Basic::needed_ench[i]}, 0, 0};
            pool.append(tm);
        }
    }
    else if(Basic::mode[1] == 1)
    {
        for(int i = 0; i < Basic::available_item_l; i++)
        {
            pool.append(Basic::available_item[i]);
        }
    }


    if(!Basic::lever[0] && !Basic::lever[1])
        additional_mode = 0;
    else if(Basic::lever[0] && !Basic::lever[1])
        additional_mode = 1;
    else if(!Basic::lever[0] && Basic::lever[1])
        additional_mode = 2;
    else
        additional_mode = 3;

//    for(int i = 0; i < list_l; i++)
//    {
//        preForge(Item({}), list[i], mode);
//    }

//    sort(list, list_l);

    qDebug() << "Calculating..";
    if(Basic::mode[0] == 0)
    {
        Alg_DifficultyFirst();
    }
    else if(Basic::mode[0] == 1)
    {
        Alg_Greedy();
    }
    else if(Basic::mode[0] == 2)
    {
        Alg_Enumeration();
    }
}


void Calculator::Alg_DifficultyFirst()
{
    /* 1、penalty相同且penalty低的组合优先
     * 2、与Weapon组合且等级花费高的优先
     * 3、等级花费高的作为目标物品优先
     * 4、等级花费高的作为牺牲物品优先
     * 5、若penalty不相同，则penalty相近的组合优先
     */

}

void Calculator::Alg_Greedy()
{

}

void Calculator::Alg_Enumeration()
{

}

