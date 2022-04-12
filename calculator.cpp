#include "calculator.h"

using namespace std;


Calculator::Calculator(QObject *parent)
    : QThread{parent}
{
    flow_l = INIT_LENGTH;
    flow = new Step[INIT_LENGTH];
    flow_step = 0;

    if(!DM->addition[0] && !DM->addition[1])
        additional_mode = ForgeMode::Normal;
    else if(DM->addition[0] && !DM->addition[1])
        additional_mode = ForgeMode::IgnoreFixing;
    else if(!DM->addition[0] && DM->addition[1])
        additional_mode = ForgeMode::IgnorePenalty;
    else
        additional_mode = ForgeMode::IgnoreFixing_Penalty;
    pool.setForgeMode(additional_mode);

    preparation();
}


void Calculator::preparation()
{
    pool.append(*DM->OriginItem);
    if(DM->itemconfig == ICM::AllLevelEBook)
    {
        for(int i = 0; i < DM->needed_ench_l; i++)
        {
            int p = BASE::sEnch(DM->origin_ench, DM->needed_ench[i].name, DM->origin_ench_l);
            Item tm = {ID_ECB, {DM->needed_ench[i]}, 0, 0};
            if(p != -1 && DM->needed_ench[i].lvl - DM->origin_ench[p].lvl == 1)
                tm.ench[0].lvl--;
            pool.append(tm);
        }
    }
    else if(DM->itemconfig == ICM::BasicEBook)
    {
        for(int i = 0; i < DM->needed_ench_l; i++)
        {
            Item tm = {ID_ECB, {DM->needed_ench[i]}, 0, 0};
            int p = BASE::sEnch(DM->origin_ench, DM->needed_ench[i].name, DM->origin_ench_l);

            int d_NO, d_NE;
            if(p == -1)
                d_NO = DM->needed_ench[i].lvl;
            else
                d_NO = tm.ench[0].lvl - DM->origin_ench[p].lvl;
            d_NE = DM->needed_ench[i].lvl - BASE::sTable(DM->needed_ench[i].name)->emlvl;

            if(d_NE <= 0)
            {
                if(d_NO == 1)
                {
                    tm.ench[0].lvl--;
                    pool.append(tm);
                }
                else
                {
                    pool.append(tm);
                }
            }
            else if(d_NE > 0)
            {
                tm.ench[0].lvl -= d_NE;
                if(d_NO == 1)
                {

                }
                for(int j = 0; j < (2^d_NE)-1; j++)
                {
                    pool.append(tm);
                }
            }
        }
    }
    else if(DM->itemconfig == ICM::AdvanceMode)
    {
        pool.cloneFrom(DM->item_pool);
    }
}

void Calculator::run()
{
    qDebug() << "Calculating...";

    if(DM->alg_mode == ALGM::DifficultyFirst)
    {
        Alg_DifficultyFirst();
    }
    else if(DM->alg_mode == ALGM::Greedy)
    {
        Alg_Greedy();
    }
    else if(DM->alg_mode == ALGM::Enumeration)
    {
        complexity = evaluateComplexity();
        if(complexity == 0)
        {
            qDebug() << "[ERROR] Calculating stop! The complexity is out of range";
            return;
        }
        Alg_Enumeration();
    }

    uploadData();
    emit isDone();
}

unsigned long long Calculator::evaluateComplexity()
{
    unsigned long long num = 1;
    for(int i = 1; i <= pool.count(); i++)
    {
        if(log2(num*i) >= 64)
            return 0;
        num *= i;
    }

    if(log2(num*num) >= 64)
        return 0;
    num *= num;

    return num;
}


void Calculator::Alg_GlobeAverage()
{

}

void Calculator::Alg_DifficultyFirst()
{
    /* 1、惩罚相同且惩罚低的组合优先
     * 2、与武器组合且等级花费高的优先
     * 3、等级花费高的作为目标物品优先
     * 4、等级花费高的作为牺牲物品优先
     * 5、若惩罚不相同，则惩罚相近的组合优先
     */

    qDebug() << "+ [Alg_DifficultyFirst]" << pool.item(0).name << pool.item(0).ench[0].name << pool.item(0).penalty << pool.item(0).durability;

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

void Calculator::Alg_Greedy()
{

}

void Calculator::Alg_Enumeration()
{

}

void Calculator::Alg_SimpleEnumeration()
{

}


void Calculator::uploadData()
{
    DM->resizeFlowList(flow_l);
    DM->upload(flow, flow_l);
    qDebug() << "The flow has been uploaded" << flow_step;
}

