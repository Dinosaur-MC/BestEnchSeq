#include "calculator.h"

using namespace std;


Calculator::Calculator(QObject *parent)
    : QObject{parent}
{
    qDebug() << "+ [Calculator]" << Basic::mode[0] << Basic::OriginItem.name << Basic::OriginItem.penalty << Basic::OriginItem.duration;
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
    else
    {
        for(int i = 0; i < Basic::available_item_l; i++)
        {
            pool.append(Basic::available_item[i]);
        }
    }


    if(!Basic::lever[0] && !Basic::lever[1])
        additional_mode = ForgeMode::Normal;
    else if(Basic::lever[0] && !Basic::lever[1])
        additional_mode = ForgeMode::IgnoreFixing;
    else if(!Basic::lever[0] && Basic::lever[1])
        additional_mode = ForgeMode::IgnorePenalty;
    else
        additional_mode = ForgeMode::IgnoreFixing_Penalty;

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
        Alg_Hamming();
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
    qDebug() << "- [Alg_DifficultyFirst]";
}

void Calculator::Alg_Hamming()
{
    qDebug() << "+ [Alg_Hamming]" << pool.item(0).name << pool.item(0).ench[0].name << pool.item(0).penalty << pool.item(0).duration;

    if (pool.count() > 1)
    {
        int mP = 0;
        QVector<QVector<Item>> tm_item_triangle, item_triangle;

        for(int i = 0; i < pool.count(); i++)
            mP = std::max(mP, pool.item(i).penalty);

        tm_item_triangle.fill(QVector<Item>({}), mP+1);

        for(int i = 0; i < pool.count(); i++)
            tm_item_triangle[pool.item(i).penalty].append(pool.item(i));

        for(int i = 0; i < tm_item_triangle.count(); i++)
        {
            while(i > item_triangle.count()-1)
                item_triangle.append(QVector<Item>({}));

            int n = tm_item_triangle.at(i).count();
            sortPool(&tm_item_triangle[i], 0, n-1);
            item_triangle[i].fill(Item(), n);

            for(int j = 0; j < n; j++)
            {
                if(tm_item_triangle.at(i).at(j).name != ID_ECB)
                {
                    item_triangle[i][0] = tm_item_triangle[i].takeAt(j);
                    break;
                }
            }
            for(int j = 1; j < n; j++)
            {
                QVector<int> tm = dupFloorMembers(j, n);
                if(tm.isEmpty())
                    break;

                for(int k = 0; k < tm.count(); k++)
                    item_triangle[i][tm.at(k)] = tm_item_triangle[i].takeFirst();
            }
            tm_item_triangle[i] = item_triangle.at(i);

            if(tm_item_triangle.at(i).count() < 2)
                continue;

            while(tm_item_triangle.at(i).count() > 1)
            {
                Item a = tm_item_triangle[i].takeFirst();
                Item b = tm_item_triangle[i].takeFirst();
                Item tm = ItemPool::forge(a, b);

                while(tm.penalty > tm_item_triangle.count()-1 || i+1 > tm_item_triangle.count()-1)
                    tm_item_triangle.append(QVector<Item>({}));

                if(tm.name != ID_ECB)
                    tm_item_triangle[i+1].append(tm);
                else
                    tm_item_triangle[tm.penalty].append(tm);
            }
            if(tm_item_triangle.at(i).count() > 0)
            {
                while(i >= tm_item_triangle.count()-1)
                    tm_item_triangle.append(QVector<Item>({}));

                tm_item_triangle[i+1].append(tm_item_triangle[i].takeFirst());
            }
        }

        for(int i = 0; i < item_triangle.count(); i++)
        {
            while(item_triangle.at(i).count() > 1)
            {
                Item a = item_triangle[i].takeFirst();
                Item b = item_triangle[i].takeFirst();
                flow[flow_step++] = ItemPool::preForge(a, b, additional_mode);
            }
            if (item_triangle.at(i).count() == 1 && i + 1 < item_triangle.count())
            {
                item_triangle[i + 1].append(item_triangle[i].takeFirst());
            }
        }
        Step s = flow[flow_step - 1];
        pool.replace(ItemPool::forge(s.a, s.b), 0);
    }
    qDebug() << "- [Alg_Hamming]";
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
