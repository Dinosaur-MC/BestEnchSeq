#include "calculator.h"

using namespace std;


Calculator::Calculator(QObject *parent)
    : QObject{parent}
{
    qDebug() << "+ [Calculator]" << Basic::mode << Basic::OriginItem.name << Basic::OriginItem.penalty << Basic::OriginItem.duration;
    //Basic::mode[1]; 0:basic mode(enchanted book only), 1:advance mode

    pool.append(Basic::OriginItem);
    if(Basic::mode[1] == 0)
    {
        for(int i = 0; i < Basic::needed_ench_l; i++)
        {
            int p = Basic::searchEnch(Basic::origin_ench, Basic::origin_ench_l, Basic::needed_ench[i].name);
            Item tm = {ID_ECB, {Basic::needed_ench[i]}, 0, 0};
            if(p != -1 && Basic::needed_ench[i].lvl - Basic::origin_ench[p].lvl == 1)
                tm.ench[0].lvl--;
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
        additional_mode = ForgeMode::IgnoreFixing;
    else if(Basic::lever[0] && !Basic::lever[1])
        additional_mode = ForgeMode::IgnorePenalty;
    else if(!Basic::lever[0] && Basic::lever[1])
        additional_mode = ForgeMode::IgnoreFixing_Penalty;
    else
        additional_mode = ForgeMode::Normal;

    pool.setForgeMode(additional_mode);

    qDebug() << "Calculating..";
    flow_l = INIT_LENGTH;
    flow = new Step[INIT_LENGTH];
    flow_step = 0;
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

    uploadData();
}


void Calculator::Alg_DifficultyFirst()
{
    /* 1、penalty相同且penalty低的组合优先
     * 2、与Weapon组合且等级花费高的优先
     * 3、等级花费高的作为目标物品优先
     * 4、等级花费高的作为牺牲物品优先
     * 5、若penalty不相同，则penalty相近的组合优先
     */

    qDebug() << "+ [Alg_DifficultyFirst]" << pool.item(0).name << pool.item(0).ench[0].name << pool.item(0).penalty << pool.item(0).duration;
//    Item item[3];

    int curPenalty = pool.item(0).penalty;
    int mode = 0;

    while(pool.count() > 1)
    {
        pool.sort();
        int begin = pool.penaltyAreaBegin(curPenalty);
        int end = pool.penaltyAreaEnd(curPenalty);
        qDebug() << flow_step << "[loop] - " << "count" << pool.count() << "cur" << curPenalty << "begin" << begin << "end" << end << "mode" << mode;

        if(mode == 0)
        {
            if(end - begin == 0)
            {
                if( curPenalty == pool.maxPenalty())
                {
                    curPenalty = pool.minPenalty();
                    mode = 1;
                }
                else
                    curPenalty++;
                continue;
            }

            int w = pool.searchWeapon();
            qDebug() << "searchWeapon" << w;
            if(pool.item(w).penalty == curPenalty)
            {
                if(w == begin)
                    begin++;
                flow[flow_step] = ItemPool::preForge(pool.item(w), pool.item(begin), additional_mode);
                pool.replace(ItemPool::forge(pool.item(w), pool.item(begin)), w);
            }
            else
            {
                flow[flow_step] = ItemPool::preForge(pool.item(begin), pool.item(begin+1), additional_mode);
                pool.replace(ItemPool::forge(pool.item(begin), pool.item(begin+1)), begin+1);
            }
            flow_step++;
            pool.remove(begin);
        }
        else
        {
            int w = pool.searchWeapon();
            qDebug() << "searchWeapon" << w;
            if(w == 1)
            {
                flow[flow_step] = ItemPool::preForge(pool.item(1), pool.item(0), additional_mode);
                pool.replace(ItemPool::forge(pool.item(1), pool.item(0)), 0);
            }
            else
            {
                flow[flow_step] = ItemPool::preForge(pool.item(0), pool.item(1), additional_mode);
                pool.replace(ItemPool::forge(pool.item(0), pool.item(1)), 0);
            }
            flow_step++;
            pool.remove(1);

            for(int i = 0, tm_pen = pool.maxPenalty(); i < tm_pen; i++)
            {
                if(pool.penaltyAreaEnd(i) - pool.penaltyAreaBegin(i) != 0)
                {
                    curPenalty = i;
                    mode = 0;
                    break;
                }
            }
        }
    }
    pool.sort();
    qDebug() << "- [Alg_DifficultyFirst]";
}

void Calculator::Alg_Greedy()
{

}

void Calculator::Alg_Enumeration()
{

}


void Calculator::uploadData()
{
    delete [] Basic::flow_list;
    Basic::flow_list_l = flow_step;
    Basic::flow_list = new Step[flow_step];
    Basic::OutputItem = pool.item(0);

    for(int i = 0; i < flow_step; i++)
    {
        Basic::flow_list[i] = flow[i];
        qDebug() << "flow_list:" << Basic::flow_list[i].a.name << Basic::flow_list[i].b.name << Basic::flow_list[i].cost << Basic::flow_list[i].penalty;
    }
    qDebug() << "The flow has been uploaded" << flow_step;
}

