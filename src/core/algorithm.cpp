#include "algorithm.h"

Algorithm::Algorithm(QObject *parent)
    : QObject{parent}
{
}

void Algorithm::start(QSet<int> es, _Item tar, _ItemPool ip, MCE mce, ALGCFG cfg)
{
    ench_set = es;
    target_item = tar;
    item_pool = ip;
    edition = mce;
    alg_config = cfg;

    if (!ench_set.isEmpty() && !item_pool.isEmpty() && edition != MCE::Null)
        run();
}

inline ulong Algorithm::levelToPoint(int lvl)
{
    if (lvl <= 16)
        return (lvl * lvl + 6 * lvl);
    else if (lvl <= 31)
        return (2.5 * lvl * lvl - 40.5 * lvl + 360);
    else
        return (4.5 * lvl * lvl - 162.5 * lvl + 2220);
}
inline int Algorithm::mergeLevel(int ench_id, int l1, int l2)
{
    return l1 == l2 ? std::min(l1 + 1, global_p_ench_table.at(ench_id).max_lvl) : std::max(l1, l2);
}

int Algorithm::findEnch(const _Item &item, const _Ench &ench, bool strict)
{
    if (strict)
    {
        for (int i = 0; i < item.enchs.size(); i++)
        {
            if (item.enchs.at(i) == ench)
                return i;
        }
    }
    else
    {
        for (int i = 0; i < item.enchs.size(); i++)
        {
            if (item.enchs.at(i).id == ench.id)
                return i;
        }
    }

    return -1;
}
QList<QPoint> Algorithm::findItemWith(int ench_id)
{
    QList<QPoint> item_pos;

    for (int i = 0; i < item_pool.size(); i++)
    {
        for (int j = 0; j < item_pool.at(i).enchs.size(); j++)
        {
            if (item_pool.at(i).enchs.at(j).id == ench_id)
                item_pos.append({i, j});
        }
    }

    return item_pos;
}

uint8_t Algorithm::checkProcessable(const _Item &item, const _Ench &ench)
{
    if (!ench_set.contains(ench.id))
        return 0x1; // 魔咒不合适

    foreach (auto &e, global_p_ench_table.at(ench.id).conflictions)
    {
        foreach (auto &ie, item.enchs)
        {
            if (ie.id == e)
                return 0x2; // 魔咒冲突
        }
    }

    return 0x0; // 检查通过
}
bool Algorithm::checkConfig(const _ItemPool &ip, QList<int> *missing)
{
    QList<_Ench> ench_l1;               // 完全匹配的魔咒的列表
    QMap<_Ench, QList<QPoint>> ench_l2; // ID匹配的魔咒的列表

    // Step 1: 查找并分别记录完全匹配和ID匹配的魔咒，如一个魔咒找不到则失败
    foreach (_Ench ench, target_item.enchs) // 遍历目标魔咒列表
    {
        int i, p = -1;
        for (i = 0; i < ip.size(); i++) // 遍历物品池
        {
            p = findEnch(ip.at(i), ench, true); // 完全查找并记录魔咒相对位置
            if (p != -1)
                break;
        }

        if (p == -1) // 魔咒不完全匹配或找不到
        {
            int n = 0;
            for (i = 0; i < ip.size(); i++) // 再次遍历物品池
            {
                p = findEnch(ip.at(i), ench, false); // ID查找并记录魔咒相对位置
                if (p != -1)                         // 找到魔咒
                {
                    n++;
                    ench_l2.insert(ench, findItemWith(ench.id));
                }
            }
            if (n < 1) // 魔咒找不到
            {
                if (missing != nullptr) // 记录缺失的魔咒
                {
                    missing->append(ench.id);
                    continue;
                }
                else // 直接失败退出
                    return false;
            }
        }
        else // 魔咒完全匹配
        {
            ench_l1.append(ench);
        }
    }
    if (missing != nullptr && !missing->isEmpty())
        return false;

    // Step 2: 尝试合并含有ID匹配的魔咒的物品并检查等级是否都符合需求，否则失败
    for (auto map = ench_l2.begin(); map != ench_l2.end(); map++)
    {
        QList<int> lvls;
        foreach (auto &p, map.value()) // 汇总等级数据
            lvls.append(item_pool[p.x()].enchs[p.y()].lvl);

        std::sort(lvls.begin(), lvls.end());  // 递增排序
        for (int i = 0; i + 1 < lvls.size();) // 等级连续合并
        {
            if (lvls[i] == lvls[i + 1])
            {
                lvls.replace(i + 1, mergeLevel(map.key().id, lvls[i], lvls[i + 1]));
                lvls.removeAt(i);
            }
            else
                i++;
        }

        if (lvls.last() < map.key().lvl) // 无法达到目标等级则失败
            return false;
    }

    return true;
}

int Algorithm::expCost(const _Item &a, const _Item &b, MCE mce, uint8_t mode)
{
    int cost = 0;

    // 计算魔咒花费
    foreach (auto &b_ench, b.enchs)
    {
        int i = checkProcessable(a, b_ench);
        if (i == 0x0) // 魔咒 合并/添加 花费
        {
            int multiplier = b.type == ItemType::Book ? global_u_ench_table.at(b_ench.id).book_multiplier : global_u_ench_table.at(b_ench.id).item_multiplier;
            int p = findEnch(a, b_ench);
            if (p > -1)
                cost += multiplier * (mce == MCE::Java ? mergeLevel(b_ench.id, a.enchs.at(p).lvl, b_ench.lvl) : mergeLevel(b_ench.id, a.enchs.at(p).lvl, b_ench.lvl) - a.enchs.at(p).lvl);
            else
                cost += multiplier * b_ench.lvl;
        }
        else if (i == 0x2) // 魔咒冲突花费
            cost += 2;
    }

    // 计算惩罚花费
    if (mode > 0x0)
        cost += pow(2, a.penalty) + pow(2, b.penalty) - 2;

    // 计算修复花费
    if (mode > 0x1)
    {
        if (b.type == ItemType::Stuff)
            cost += b.durability; // 此时 durability 表示材料数量
        else if (b.type == ItemType::Weapon)
            cost += 2;
    }

    return cost;
}

_Item Algorithm::forge(const _Item &a, const _Item &b)
{
    _Item it = a;

    // 合并魔咒
    foreach (auto &ench, b.enchs)
    {
        if (checkProcessable(it, ench))
        {
            int p = findEnch(it, ench);
            if (p > -1)
                it.enchs[p] = _Ench(ench.id, mergeLevel(ench.id, it.enchs.at(p).lvl, ench.lvl));
            else
                it.enchs.append(ench);
        }
    }

    // 计算惩罚
    it.penalty = it.penalty == b.penalty ? it.penalty + 1 : std::max(it.penalty, b.penalty) + 1;

    // 计算耐久度（%）
    if (it.type == ItemType::Weapon)
    {
        if (b.type == ItemType::Stuff)
            it.durability = std::min(it.durability + 25 * b.durability, 100); // 此时 b.durability 表示材料数量
        else if (it.durability < 100)
            it.durability = std::min(it.durability + b.durability + 12, 100);
    }

    return it;
}

void FinTriangle::update(int n)
{
    data.clear();
    for (int i = 0; i < n; i++)
    {
        int n = 0;
        for (int j = i; j > 0; j /= 2)
        {
            if (j % 2 == 1)
                n++;
        }
        data.append(n);
    }
}
int FinTriangle::vaule(int x)
{
    if (x < data.size())
        return data.at(x);
    else
        return -1;
}
QList<int> FinTriangle::arcValues(int x)
{
    QList<int> values;
    for (int i = 0; i < data.size(); i++)
    {
        if (data.at(i) == x)
            values.append(i);
    }
    return values;
}
