#include "itempool.h"

using namespace std;

#define INIT_SIZE 16

ItemPool::ItemPool(QObject* parent)
    : QObject { parent }
{
    storage_mode = StorageMode::AutoSize;
    item_count = 0;
    pool_l = INIT_SIZE;
    pool = new Item[pool_l];
    forgeMode = ForgeMode::Normal;
}

void ItemPool::setStorageMode(enum StorageMode m)
{
    if (storage_mode != m) {
        storage_mode = m;
        detectSize();
    }
}

int ItemPool::detectSize()
{
    if (storage_mode == StorageMode::AutoSize) {
        int d_size = pool_l - item_count;
        if (d_size == 0) {
            int tm_l = pool_l;
            Item* tm = new Item[tm_l];
            for (int i = 0; i < tm_l; i++)
                tm[i] = pool[i];

            delete[] pool;

            pool_l += INIT_SIZE;
            pool = new Item[pool_l];
            for (int i = 0; i < tm_l && i < pool_l; i++)
                pool[i] = tm[i];

            delete[] tm;
        } else if (d_size > 2 * INIT_SIZE) {
            int tm_l = pool_l;
            Item* tm = new Item[tm_l];
            for (int i = 0; i < tm_l; i++)
                tm[i] = pool[i];

            delete[] pool;

            pool_l -= (d_size / INIT_SIZE - 1) * INIT_SIZE;
            pool = new Item[pool_l];
            for (int i = 0; i < tm_l && i < pool_l; i++)
                pool[i] = tm[i];

            delete[] tm;
        }
    } else if (storage_mode == StorageMode::FixedSize) {
        int d_size = pool_l - item_count;
        return d_size;
    } else if (storage_mode == StorageMode::MinimunSize) {
        int d_size = item_count - pool_l;

        if (d_size != -1) {
            int tm_l = pool_l;
            Item* tm = new Item[tm_l];
            for (int i = 0; i < tm_l; i++)
                tm[i] = pool[i];

            delete[] pool;

            pool_l += d_size + 1;
            pool = new Item[pool_l];
            for (int i = 0; i < tm_l && i < pool_l; i++)
                pool[i] = tm[i];

            delete[] tm;
        }
    }

    return pool_l - item_count;
}

void ItemPool::resize(int size)
{
    int tm_l = pool_l;
    Item* tm = new Item[tm_l];
    for (int i = 0; i < tm_l; i++)
        tm[i] = pool[i];

    delete[] pool;

    pool_l = size;
    pool = new Item[pool_l];
    for (int i = 0; i < tm_l && i < pool_l; i++)
        pool[i] = tm[i];

    delete[] tm;
}

void ItemPool::insert(Item item, int p)
{
    if (!detectSize() || p > pool_l - 1)
        return;

    for (int i = p; i < item_count + 1; i++)
        pool[i + 1] = pool[i];

    pool[p] = item;
    item_count++;
}

void ItemPool::append(Item item)
{
    if (!detectSize())
        return;

    pool[item_count] = item;
    item_count++;
}

void ItemPool::replace(Item item, int p)
{
    if (p > pool_l - 1)
        return;
    pool[p] = item;
}

void ItemPool::remove(Item item)
{
    int p;
    for (p = 0; p < item_count; p++) {
        if (Basic::compareItem(pool[p], item) == 0) {
            int j;
            for (j = p; j < item_count - 1; j++) {
                pool[j] = pool[j + 1];
            }
            pool[j] = Item({});
            break;
        }
        if (p == item_count - 1)
            return;
    }

    item_count--;
    detectSize();
}
void ItemPool::remove(int p)
{
    if (p > item_count - 1)
        return;
    int i;
    for (i = p; i < item_count - 1; i++) {
        pool[i] = pool[i + 1];
    }
    pool[i] = Item({});

    item_count--;
    detectSize();
}

void ItemPool::sort()
{
    qDebug() << "+ sort";

    for (int i = 0; i < item_count - 1; i++) {
        for (int j = 0; j < item_count - 1 - i; j++) {
            if (pool[j].penalty > pool[j + 1].penalty) {
                Item tm = pool[j + 1];
                pool[j + 1] = pool[j];
                pool[j] = tm;
            } else if (pool[j].penalty == pool[j + 1].penalty
                && preForge(Item({}), pool[j], ForgeMode::IgnorePenalty).cost < preForge(Item({}), pool[j + 1], ForgeMode::IgnorePenalty).cost
                && (pool[j].name == ID_ECB || pool[j].name == "")) {
                if (preForge(Item({}), pool[j], ForgeMode::IgnorePenalty).cost < preForge(Item({}), pool[j + 1], ForgeMode::IgnorePenalty).cost
                    && (pool[j].name == ID_ECB || pool[j].name == "")) {
                    Item tm = pool[j + 1];
                    pool[j + 1] = pool[j];
                    pool[j] = tm;
                }
            }
        }
    }

    for (int i = 0; i < item_count; i++) {
        qDebug() << "item - n e p c" << pool[i].name << pool[i].ench[0].name << pool[i].penalty << preForge(Item({}), pool[i], ForgeMode::IgnorePenalty).cost;
    }

    qDebug() << "- sort";
}

int ItemPool::searchWeapon()
{
    int p;
    for (p = 0; p < item_count; p++) {
        if (pool[p].name != ID_ECB && pool[p].name != "")
            break;
        if (p == item_count - 1) {
            p = -1;
            break;
        }
    }
    return p;
}

int ItemPool::penaltyAreaBegin(int pen)
{
    int p = 0;
    while (pool[p].penalty < pen && p < item_count)
        p++;
    if (pool[p].penalty != pen)
        return -1;
    return p;
}

int ItemPool::penaltyAreaEnd(int pen)
{
    int p = 0;
    while (pool[p].penalty <= pen && p < item_count)
        p++;
    p--;
    if (pool[p].penalty != pen)
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
    if (p < item_count)
        return pool[p];
    else
        return Item({});
}

int ItemPool::maxPenalty()
{
    int pen = pool[0].penalty;
    for (int i = 1; i < item_count; i++) {
        pen = max(pool[i].penalty, pen);
    }
    return pen;
}

int ItemPool::minPenalty()
{
    int pen = pool[0].penalty;
    for (int i = 1; i < item_count; i++) {
        pen = min(pool[i].penalty, pen);
    }
    return pen;
}

int ItemPool::maxLevelCost()
{
    int lCost = preForge(Item({}), pool[0], ForgeMode::IgnoreFixing_Penalty).cost;
    for (int i = 1; i < item_count; i++) {
        lCost = max(preForge(Item({}), pool[i], ForgeMode::IgnoreFixing_Penalty).cost, lCost);
    }
    return lCost;
}

int ItemPool::minLevelCost()
{
    int lCost = preForge(Item({}), pool[0], ForgeMode::IgnoreFixing_Penalty).cost;
    for (int i = 1; i < item_count; i++) {
        lCost = min(preForge(Item({}), pool[i], ForgeMode::IgnoreFixing_Penalty).cost, lCost);
    }
    return lCost;
}

int ItemPool::maxCost()
{
    int lCost = preForge(Item({}), pool[0], ForgeMode::Normal).cost;
    for (int i = 1; i < item_count; i++) {
        lCost = max(preForge(Item({}), pool[i], ForgeMode::Normal).cost, lCost);
    }
    return lCost;
}

int ItemPool::minCost()
{
    int lCost = preForge(Item({}), pool[0], ForgeMode::Normal).cost;
    for (int i = 1; i < item_count; i++) {
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
    while (A_el < INIT_LENGTH && A.ench[A_el].name != "")
        A_el++;
    while (B_el < INIT_LENGTH && B.ench[B_el].name != "")
        B_el++;

    for (int i = 0; i < B_el; i++) {
        int lever = 0;
        int p = Basic::searchTable(B.ench[i].name);
        for (int j = 0; j < Basic::ench_table[p].repulsion->size(); j++) {
            if (Basic::searchEnch(A.ench, A_el, Basic::ench_table[p].repulsion[j]) != -1) {
                if (Basic::edition == 0)
                    cost += 1;
                lever++;
                break;
            }
        }
        if (lever)
            continue;

        int q = Basic::searchEnch(A.ench, A_el, B.ench[i].name);
        if (q != -1) {
            if (Basic::edition == 0) {
                if (B.name == ID_ECB)
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl);
                else
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[0] * combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl);
            } else {
                if (B.name == ID_ECB)
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * (combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl) - A.ench[q].lvl);
                else
                    cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[0] * (combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl) - A.ench[q].lvl);
            }
        } else {
            if (B.name == ID_ECB)
                cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[1] * B.ench[i].lvl;
            else
                cost += Basic::ench_table[Basic::searchTable(B.ench[i].name)].multiplier[0] * B.ench[i].lvl;
        }
    }

    if (mode != ForgeMode::IgnorePenalty && mode != ForgeMode::IgnoreFixing_Penalty) {
        cost += pow(2, A.penalty) - 1;
        cost += pow(2, B.penalty) - 1;
    }
    if ((mode != ForgeMode::IgnoreFixing && mode != ForgeMode::IgnoreFixing_Penalty) && A.duration != 100 && B.duration != 0)
        cost += 2;

    s.cost = cost;
    s.penalty = penalty;
    //    qDebug() << "preForge:" << cost << penalty << A_el << A.ench[0].name << A.ench[0].lvl << B_el << B.ench[0].name << B.ench[0].lvl;
    return s;
}

int ItemPool::preForge(Item it, ForgeMode mode)
{
    int el = 0;
    while (el < INIT_LENGTH && it.ench[el].name != "")
        el++;

    int cost = 0;
    for (int i = 0; i < el; i++) {
        int p = Basic::searchTable(it.ench[i].name);
        if (it.name == ID_ECB)
            cost += Basic::ench_table[p].multiplier[1] * it.ench[i].lvl;
        else
            cost += Basic::ench_table[p].multiplier[0] * it.ench[i].lvl;
    }
    if (mode != ForgeMode::IgnorePenalty && mode != ForgeMode::IgnoreFixing_Penalty)
        cost += pow(2, it.penalty) - 1;

    return cost;
}

Item ItemPool::forge(Item A, Item B)
{
    int A_el = 0, B_el = 0;
    while (A_el < INIT_LENGTH && A.ench[A_el].name != "")
        A_el++;
    while (B_el < INIT_LENGTH && B.ench[B_el].name != "")
        B_el++;

    for (int i = 0; i < INIT_LENGTH && i < B_el; i++) {
        int lever = 0;
        int p = Basic::searchTable(B.ench[i].name);
        for (int j = 0; j < Basic::ench_table[p].repulsion->size(); j++) {
            if (Basic::searchEnch(A.ench, A_el, Basic::ench_table[p].repulsion[j]) != -1) {
                lever++;
                break;
            }
        }
        if (lever)
            continue;

        int q = Basic::searchEnch(A.ench, A_el, B.ench[i].name);
        if (q != -1) {
            A.ench[q].lvl = combine(B.ench[i].name, A.ench[q].lvl, B.ench[i].lvl);
        } else {
            int k = 0;
            while (k < INIT_LENGTH && A.ench[k].name != "")
                k++;
            if (A.ench[k].name == "")
                A.ench[k] = B.ench[i];
        }
    }

    if (A.duration != 100 && B.duration != 0) {
        A.duration += B.duration;
        A.duration *= 1.12;
        if (A.duration > 100)
            A.duration = 100;
    }
    A.penalty = max(A.penalty, B.penalty) + 1;

    return A;
}

int combine(QString ench, int a, int b)
{
    if (a == b && a != Basic::ench_table[Basic::searchTable(ench)].mlvl)
        return a + 1;
    else
        return max(a, b);
}
