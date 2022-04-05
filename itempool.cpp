#include "itempool.h"
#include "datamanager.h"

using namespace std;

#define INIT_POOLSIZE 16

ItemPool::ItemPool()
{
    storage_mode = StorageMode::AutoSize;
    forgeMode = ForgeMode::Normal;
    item_count = 0;
    pool_l = INIT_POOLSIZE;
    pool = new Item[pool_l];
}


void ItemPool::setStorageMode(enum StorageMode m)
{
    if(storage_mode != m)
    {
        storage_mode = m;
        detectSize();
    }
}

int ItemPool::detectSize()
{
    if(storage_mode == StorageMode::AutoSize)
    {
        int d_size = pool_l - item_count;
        if(d_size == 0)
        {
            int tm_l = pool_l;
            Item *tm = new Item[tm_l];
            for(int i = 0; i < tm_l; i++)
                tm[i] = pool[i];

            delete [] pool;

            pool_l += INIT_POOLSIZE;
            pool = new Item[pool_l];
            for(int i = 0; i < tm_l && i < pool_l; i++)
                pool[i] = tm[i];

            delete [] tm;
        }
        else if(d_size > 2*INIT_POOLSIZE)
        {
            int tm_l = pool_l;
            Item *tm = new Item[tm_l];
            for(int i = 0; i < tm_l; i++)
                tm[i] = pool[i];

            delete [] pool;

            pool_l -= (d_size/INIT_POOLSIZE - 1) * INIT_POOLSIZE;
            pool = new Item[pool_l];
            for(int i = 0; i < tm_l && i < pool_l; i++)
                pool[i] = tm[i];

            delete [] tm;
        }
    }
    else if(storage_mode == StorageMode::FixedSize)
    {
        int d_size = pool_l - item_count;
        return d_size;
    }
    else if(storage_mode == StorageMode::MinimunSize)
    {
        int d_size = item_count - pool_l;

        if(d_size != -1)
        {
            int tm_l = pool_l;
            Item *tm = new Item[tm_l];
            for(int i = 0; i < tm_l; i++)
                tm[i] = pool[i];

            delete [] pool;

            pool_l += d_size+1;
            pool = new Item[pool_l];
            for(int i = 0; i < tm_l && i < pool_l; i++)
                pool[i] = tm[i];

            delete [] tm;
        }
    }

    return pool_l - item_count;
}

void ItemPool::resize(int size)
{
    int tm_l = pool_l;
    Item *tm = new Item[tm_l];
    for(int i = 0; i < tm_l; i++)
        tm[i] = pool[i];

    delete [] pool;

    pool_l = size;
    pool = new Item[pool_l];
    for(int i = 0; i < tm_l && i < pool_l; i++)
        pool[i] = tm[i];

    delete [] tm;
}


void ItemPool::insert(Item item, int p)
{
    if(!detectSize() || p > pool_l-1)
        return;

    for(int i = p; i < item_count+1; i++)
        pool[i+1] = pool[i];

    pool[p] = item;
    item_count++;
}

void ItemPool::append(Item item)
{
    if(!detectSize())
        return;

    pool[item_count] = item;
    item_count++;
}
void ItemPool::append(QString name, Ench *ench, int ench_l, int dur, int pen)
{
    if(!detectSize())
        return;

    Item item = {name, {}, dur, pen};
    for(int i = 0; i < ench_l && i < INIT_LENGTH; i++)
    {
        item.ench[i] = ench[i];
    }
    pool[item_count] = item;
    item_count++;
}

void ItemPool::replace(Item item, int p)
{
    if(p > pool_l-1)
        return;
    pool[p] = item;
}


void ItemPool::remove(Item item)
{
    int p;
    for(p = 0; p < item_count; p++)
    {
        if(pool[p] == item)
        {
            int j;
            for(j = p; j < item_count-1; j++)
            {
                pool[j] = pool[j+1];
            }
            pool[j] = Item({});
            break;
        }
        if(p == item_count - 1)
            return;
    }

    item_count--;
    detectSize();
}
void ItemPool::remove(int p)
{
    if(p > item_count-1)
        return;
    int i;
    for(i = p; i < item_count-1; i++)
    {
        pool[i] = pool[i+1];
    }
    pool[i] = Item({});

    item_count--;
    detectSize();
}
void ItemPool::clear()
{
    item_count = 0;
    delete [] pool;
    pool_l = INIT_POOLSIZE;
    pool = new Item[pool_l];
}


void ItemPool::sort()
{
    qDebug() << "+ sort";

    for(int i = 0; i < item_count-1; i++)
    {
        for(int j = 0; j < item_count-1-i; j++)
        {
            if(pool[j].penalty > pool[j+1].penalty)
            {
                Item tm = pool[j+1];
                pool[j+1] = pool[j];
                pool[j] = tm;
            }
            else if(pool[j].penalty == pool[j+1].penalty \
                    && preForge(Item({}), pool[j], ForgeMode::IgnorePenalty).cost < preForge(Item({}), pool[j+1], ForgeMode::IgnorePenalty).cost \
                    && (pool[j].name == ID_ECB || pool[j].name == ""))
            {
                if(preForge(Item({}), pool[j], ForgeMode::IgnorePenalty).cost < preForge(Item({}), pool[j+1], ForgeMode::IgnorePenalty).cost \
                    &&(pool[j].name == ID_ECB || pool[j].name == ""))
                {
                    Item tm = pool[j+1];
                    pool[j+1] = pool[j];
                    pool[j] = tm;
                }
            }
        }
    }

    for(int i = 0; i < item_count; i++)
    {
        qDebug() << "item - n e p c" << pool[i].name << pool[i].ench[0].name << pool[i].penalty << preForge(Item({}), pool[i], ForgeMode::IgnorePenalty).cost;
    }

    qDebug() << "- sort";
}

int ItemPool::searchWeapon()
{
    int p;
    for(p = 0; p < item_count; p++)
    {
        if(pool[p].name != ID_ECB && pool[p].name != "")
            break;
        if(p == item_count - 1)
        {
            p = -1;
            break;
        }
    }
    return p;
}

int ItemPool::penaltyAreaBegin(int pen)
{
    int p = 0;
    while(pool[p].penalty < pen && p < item_count)
        p++;
    if(pool[p].penalty != pen)
        return -1;
    return p;
}

int ItemPool::penaltyAreaEnd(int pen)
{
    int p = 0;
    while(pool[p].penalty <= pen && p < item_count)
        p++;
    p--;
    if(pool[p].penalty != pen)
        return -1;
    return p;
}

int ItemPool::count()
{
    return item_count;
}

int ItemPool::size()
{
    return pool_l;
}

Item ItemPool::item(int p)
{
    if(p < item_count)
        return pool[p];
    else
        return Item({});
}


int ItemPool::maxPenalty()
{
    int pen = pool[0].penalty;
    for(int i = 1; i < item_count; i++)
    {
        pen = max(pool[i].penalty, pen);
    }
    return pen;
}

int ItemPool::minPenalty()
{
    int pen = pool[0].penalty;
    for(int i = 1; i < item_count; i++)
    {
        pen = min(pool[i].penalty, pen);
    }
    return pen;
}


int ItemPool::maxLevelCost()
{
    int lCost = preForge(Item({}), pool[0], ForgeMode::IgnoreFixing_Penalty).cost;
    for(int i = 1; i < item_count; i++)
    {
        lCost = max(preForge(Item({}), pool[i], ForgeMode::IgnoreFixing_Penalty).cost, lCost);
    }
    return lCost;
}

int ItemPool::minLevelCost()
{
    int lCost = preForge(Item({}), pool[0], ForgeMode::IgnoreFixing_Penalty).cost;
    for(int i = 1; i < item_count; i++)
    {
        lCost = min(preForge(Item({}), pool[i], ForgeMode::IgnoreFixing_Penalty).cost, lCost);
    }
    return lCost;
}


int ItemPool::maxCost()
{
    int lCost = preForge(Item({}), pool[0], ForgeMode::Normal).cost;
    for(int i = 1; i < item_count; i++)
    {
        lCost = max(preForge(Item({}), pool[i], ForgeMode::Normal).cost, lCost);
    }
    return lCost;
}

int ItemPool::minCost()
{
    int lCost = preForge(Item({}), pool[0], ForgeMode::Normal).cost;
    for(int i = 1; i < item_count; i++)
    {
        lCost = min(preForge(Item({}), pool[i], ForgeMode::Normal).cost, lCost);
    }
    return lCost;
}


void ItemPool::setForgeMode(ForgeMode mode)
{
    forgeMode = mode;
}

Step ItemPool::preForge(Item A, Item B, ForgeMode mode)
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

    for(int i = 0; i < B_el; i++)
    {
        int lever = 0;
        struct EnchTable *p = BASE::sTable(B.ench[i].name);
        for(int j = 0; j < p->repulsion->count(); j++)
        {
            if(BASE::sEnch(A.ench, p->repulsion[j], A_el) != -1)
            {
                if(DM->edition == 0)
                    cost += 1;
                lever++;
                break;
            }
        }
        if(lever)
            continue;

        int q = BASE::sEnch(A.ench, B.ench[i].name, A_el);
        if(q != -1)
        {
            if(DM->edition == 0)
            {
                if(B.name == ID_ECB)
                    cost += BASE::sTable(B.ench[i].name)->multiplier[1] * combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl);
                else
                    cost += BASE::sTable(B.ench[i].name)->multiplier[0] * combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl);
            }
            else
            {
                if(B.name == ID_ECB)
                    cost += BASE::sTable(B.ench[i].name)->multiplier[1] * (combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl - A.ench[i].lvl));
                else
                    cost += BASE::sTable(B.ench[i].name)->multiplier[0] * (combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl - A.ench[i].lvl));
            }
        }
        else
        {
            if(B.name == ID_ECB)
                cost += BASE::sTable(B.ench[i].name)->multiplier[1] * B.ench[i].lvl;
            else
                cost += BASE::sTable(B.ench[i].name)->multiplier[0] * B.ench[i].lvl;
        }
    }

    if(mode != ForgeMode::IgnorePenalty && mode != ForgeMode::IgnoreFixing_Penalty)
    {
        cost += pow(2, A.penalty) - 1;
        cost += pow(2, B.penalty) - 1;
    }
    if((mode != ForgeMode::IgnoreFixing && mode != ForgeMode::IgnoreFixing_Penalty) && A.durability != 100 && B.durability != 0)
        cost += 2;

    s.cost = cost;
    s.penalty = penalty;
//    qDebug() << "preForge:" << cost << penalty << A_el << A.ench[0].name << A.ench[0].lvl << B_el << B.ench[0].name << B.ench[0].lvl;
    return s;
}

Item ItemPool::forge(Item A, Item B)
{
    int A_el = 0, B_el = 0;
    while(A_el < INIT_LENGTH && A.ench[A_el].name != "")
        A_el++;
    while(B_el < INIT_LENGTH && B.ench[B_el].name != "")
        B_el++;

    for(int i = 0; i < INIT_LENGTH && i < B_el; i++)
    {
        int lever = 0;
        struct EnchTable *t = BASE::sTable(B.ench[i].name);
        for(int j = 0; j < t->repulsion->count(); j++)
        {
            if(BASE::sEnch(A.ench, t->repulsion[j], A_el) != -1)
            {
                lever++;
                break;
            }
        }
        if(lever)
            continue;

        int q = BASE::sEnch(A.ench, B.ench[i].name, A_el);
        if(q != -1)
        {
            A.ench[q].lvl = combine(B.ench[i].name, A.ench[q].lvl, B.ench[q].lvl);
//            if(A.ench[q].lvl == B.ench[i].lvl && A.ench[q].lvl != DM->ench_table[DM->searchTable(A.ench[q].name)].mlvl)
//               A.ench[q].lvl++;
//            else if(A.ench[q].lvl < B.ench[i].lvl)
//                A.ench[q].lvl = B.ench[i].lvl;
        }
        else
        {
            int k = 0;
            while(k < INIT_LENGTH && A.ench[k].name != "")
                k++;
            if(A.ench[k].name == "")
                A.ench[k] = B.ench[i];
        }
    }

    if(A.durability != 100 && B.durability != 0)
    {
        A.durability += B.penalty;
        A.durability *= 1.12;
        if(A.durability > 100)
            A.durability = 100;
    }
    A.penalty = max(A.penalty, B.penalty) + 1;

    return A;
}

int combine(QString ench, int a, int b)
{
    if(a == b && a != BASE::sTable(ench)->mlvl)
        return ++a;
    else
        return max(a, b);
}

