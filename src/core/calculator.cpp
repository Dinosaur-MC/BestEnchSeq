#include "calculator.h"

int penaltyCost(int x)  // 计算惩罚数的等级花费
{
    return pow(2, x) - 1;
}

int duplicationSeries(int x)    // 右值频次数列
{
    // 定义域和值域都是自然数集（ x,y∈ N）

    if(x < 0)   // 界定定义域为自然数，超出范围则返回错误代码 -1
        return -1;

    int y = 0;
    for(int j = x; j > 0; j/=2) // 计算右值频次
    {
        if(j%2 == 1)
            y++;
    }

    return y;
}

QVector<int> dupFloorMembers(int x, int n)
{
    QVector<int> y;
    for(int i = 0; i < n; i++)
    {
        if(x == duplicationSeries(i))
            y.append(i);
    }
    return y;
}


Calculator::Calculator(Core *c, QObject *parent)
    : QThread{parent},
      mdb("Calculator")
{
    core = c;

    /* ========================================
     *
     * // 注意事项 //
     * 1、初始化计算模块类前要确保 core 的池数据正确有效
     * 2、计算前要写入数据到 input_item 和 output_item
     * 3、计算前记得载入数据到 item_pool
     *
     * ========================================*/
}

Calculator::~Calculator()
{

}

void Calculator::initialize()
{
    // 写入默认计算配置
    edition = core->config.default_edition;
    icm = core->config.default_itemconfig;
    algm = core->config.default_algorithm;
    pfaddn = PFADDN::Normal;

    // 格式化池
    for(int i = 0; i < core->ench_table.count(); i++)
        eps.append(formatToLocal(core->ench_table.at(i)));
    for(int i = 0; i < core->weapon_table.count(); i++)
        wps.append(formatToLocal(core->weapon_table.at(i)));
}


// 格式转换函数
_Ench Calculator::formatToLocal(const Ench &e)
{
    _Ench tm;
    for(int i = 0; i < core->ench_table.count(); i++)
    {
        if(e.name == core->ench_table.at(i).name)
        {
            tm.id = i;
            break;
        }
    }
    tm.lvl = e.lvl;
    return tm;
}

_EnchPlus Calculator::formatToLocal(const EnchPlus &ep)
{
    _EnchPlus tm;
    for(int i = 0; i < core->ench_table.count(); i++)
    {
        if(ep.name == core->ench_table.at(i).name)
        {
            tm.id = i;
            tm.max_level = core->ench_table.at(i).max_level;
            tm.poor_max_level = core->ench_table.at(i).poor_max_level;
            tm.multiplier[0] = core->ench_table.at(i).multiplier[0];
            tm.multiplier[1] = core->ench_table.at(i).multiplier[1];
            tm.edition = core->ench_table.at(i).edition;
            for(int j = 0; j < ep.repulsion.count(); j++)
            {
                for(int k = 0; k < core->ench_table.count(); k++)
                {
                    if(ep.repulsion.at(j) == core->ench_table.at(k).name)
                    {
                        tm.repulsion.append(k);
                        break;
                    }
                }
            }
            break;
        }
    }
    return tm;
}

_Weapon Calculator::formatToLocal(const WeaponPlus &wp)
{
    _Weapon tm;
    for(int i = 0; i < core->weapon_table.count(); i++)
    {
        if(wp.name == core->weapon_table.at(i).name)
        {
            tm.id = i;
            for(int j = 0; j < wp.suitableE.count(); j++)
            {
                tm.suitableE.append(formatToLocal(wp.suitableE.at(j)));
            }
            break;
        }
    }
    return tm;
}

_Item Calculator::formatToLocal(const Item &it)
{
    _Item tm;
    int p = -1;
    for(int i = 0; i < core->weapon_table.count(); i++)
    {
        if(it.name == core->weapon_table.at(i).name)
        {
            p = i;
            break;
        }
    }
    tm.id = p;

    tm.type = it.type;

    for(int i = 0; i < it.ench.count(); i++)
    {
        tm.ench.append(formatToLocal(it.ench.at(i)));
    }

    tm.durability = it.durability;
    tm.penalty = it.penalty;
    return tm;
}

_FlowStep Calculator::formatToLocal(const FlowStep &fs)
{
    return {formatToLocal(fs.a), formatToLocal(fs.b), fs.levelCost, fs.pointCost};
}

Item Calculator::formatToCore(const _Item &it)
{
    Item tm;
    if(it.type == ItemType::enchanted_book)
    {
        tm.name = ID_ECB;
        tm.icon = QIcon(ICON_ECB);
    }
    else if(it.type == ItemType::weapon && it.id > -1)
    {
        tm.name = core->weapon_table.at(it.id).name;
        tm.icon = core->weapon_table.at(it.id).icon;
    }
    else// if(it.type == ItemType::consumable)
    {
        // 新版预留，敬请期待
        tm.name = "<Unknow>";
        tm.icon = QIcon(":/icon/pack.png");
    }
    tm.type = it.type;
    for(int i = 0; i < it.ench.count(); i++)
    {
        tm.ench.append({core->ench_table.at(it.ench.at(i).id).name, it.ench.at(i).lvl});
    }
    tm.durability = it.durability;
    tm.penalty = it.penalty;
    return tm;
}

FlowStep Calculator::formatToCore(const _FlowStep &fs)
{
    return {formatToCore(fs.a), formatToCore(fs.b), fs.levelCost, fs.pointCost};
}


// 基本操作函数
void Calculator::setEdition(MCE v)
{
    edition = v;
}

void Calculator::setICM(ICM v)
{
    icm = v;
}

void Calculator::setALGM(ALGM v)
{
    algm = v;
}

void Calculator::setPFADDN(PFADDN v)
{
    pfaddn = v;
}

void Calculator::setInputItem(const _Item &it)
{
    input_item = it;
}

void Calculator::setInputItem(const Item &it)
{
    input_item = formatToLocal(it);
}

void Calculator::setOutputItem(const _Item &it)
{
    output_item = it;
}

void Calculator::setOutputItem(const Item &it)
{
    output_item = formatToLocal(it);
}

void Calculator::setItemPool(const QVector<_Item> &ip)
{
    item_pool = ip;
}

void Calculator::setItemPool(const QVector<Item> &ip)
{
    item_pool.clear();
    for(int i = 0; i < ip.count(); i++)
        item_pool.append(formatToLocal(ip.at(i)));
}

_Item Calculator::poolAt(int p)
{
    return item_pool.at(p);
}

void Calculator::appendInPool(const _Item &it)
{
    item_pool.append(it);
}

void Calculator::appendInPool(const Item &it)
{
    item_pool.append(formatToLocal(it));
}

void Calculator::prependInPool(const _Item &it)
{
    item_pool.prepend(it);
}

void Calculator::prependInPool(const Item &it)
{
    item_pool.prepend(formatToLocal(it));
}

void Calculator::removeFromPool(int p, int n)
{
    item_pool.remove(p, n);
}

void Calculator::removePoolFirst()
{
    item_pool.removeFirst();
}

void Calculator::removePoolLast()
{
    item_pool.removeLast();
}

void Calculator::clearPool()
{
    item_pool.clear();
}

int Calculator::poolItemCount()
{
    return item_pool.count();
}

bool Calculator::optimizePool()
{
    dropped_item.clear();
    for(int i = 0; i < item_pool.count(); i++)
    {
        if(integratingDegree(item_pool.at(i)) == 0)
        {
            dropped_item.append(item_pool.takeAt(i));
            i--;
        }
    }

    _sort_1(0, item_pool.count()-1);
    _Item it = input_item;
    for(int i = item_pool.count()-1; i > -1; i--)
    {
        forge(&it, item_pool.at(i));
    }

    for(int i = 0; i < output_item.ench.count(); i++)
    {
        if(getContentIndex(it.ench, output_item.ench.at(i)) < 0)
        {
            return false;
        }
    }

    return true;
}

void Calculator::sortPool()
{
    if(item_pool.count() > 1)
        sortPool(&item_pool, 0, item_pool.count()-1);

    for(int i = 0; i < item_pool.count(); i++)
    {
        QString str = "\n["+QString::number(i)+"] " + formatToCore(item_pool.at(i)).name + " P:" + QString::number(item_pool.at(i).penalty) + ", C:"+QString::number(preForge(item_pool.at(i)))+ "\n";
        for(int j = 0; j < item_pool.at(i).ench.count(); j++)
        {
            str += " - ["+QString::number(j+1)+"]" + core->ench_table.at(item_pool.at(i).ench.at(j).id).name + " " + QString::number(item_pool.at(i).ench.at(j).lvl);
            if(j < item_pool.at(i).ench.count()-1)
                str += "\n";
        }
        mdb.msg(str);
    }
    mdb.msg("排序完成！\n");
}

int Calculator::getContentIndex(const QVector<int> ns, const int &n)
{
    int p = -1;
    for(int i = 0; i < ns.count(); i++)
    {
        if(ns.at(i) == n)
        {
            p = i;
            break;
        }
    }
    return p;
}

int Calculator::getContentIndex(const QVector<_Ench> &es, const _Ench &e, bool strict)
{
    int p = -1;
    for(int i = 0; i < es.count(); i++)
    {
        if(es.at(i).id == e.id && (es.at(i).lvl == e.lvl || !strict))
        {
            p = i;
            break;
        }
    }
    return p;
}

void Calculator::getResult()
{
    core->flow.clear();
    for(int i = 0; i < flow.count(); i++)
    {
        core->flow.append(formatToCore(flow.at(i)));
    }
}


// 基本运算函数
int Calculator::integratingDegree(const _Item &it)
{
    int n = 0;
    for(int i = 0; i < it.ench.count(); i++)
    {
        if(getContentIndex(output_item.ench, it.ench.at(i), false) > -1)
            n++;
    }
    for(int i = 0; i < output_item.ench.count(); i++)
    {
        QVector<int> epr_id = eps.at(output_item.ench.at(i).id).repulsion;
        for(int j = 0; j < it.ench.count(); j++)
        {
            for(int k = 0; k < epr_id.count(); k++)
            {
                if(it.ench.at(j).id == epr_id.at(k))
                {
                    n = -n;
                    i = output_item.ench.count();
                    j = it.ench.count();
                    break;
                }
            }
        }
    }
    return n;
}

bool Calculator::checkCombinable(const _Item &it, const _Ench &e, bool strict)
{
    if(it.type == ItemType::weapon && it.id > -1)
    {
        bool u = true;
        for(int i = 0; i < wps.at(it.id).suitableE.count(); i++)
        {
            if(e.id == wps.at(it.id).suitableE.at(i).id)
            {
                u = false;
                break;
            }
        }
        if(u)
            return false;
    }
    else if(it.type == ItemType::consumable)
        return false;

    if(strict)
    {
        for(int i = 0; i < it.ench.count(); i++) // 检查魔咒是否冲突
        {
            QVector<int> epr_id = eps.at(it.ench.at(i).id).repulsion;
            for(int j = 0; j < epr_id.count(); j++)
            {
                if(e.id == epr_id.at(j))
                    return false;
            }
        }
    }

    return true;
}

int Calculator::preForge(const _Item &a, const _Item &b, bool strict)
{
    int cost = 0;

    if(strict)
        cost += penaltyCost(a.penalty) + penaltyCost(b.penalty);    // 计算累积惩罚

    for(int i = 0; i < b.ench.count(); i++) // 遍历 b
    {
        if(checkCombinable(a, b.ench.at(i), false)) // 检查适配性
        {
            // 检查冲突
            bool u = false;
            QVector<int> epr_id = eps.at(b.ench.at(i).id).repulsion;
            for(int j = 0; j < epr_id.count(); j++) // 遍历 b 的 repulsion
            {
                for(int k = 0; k < a.ench.count(); k++) // 遍历 a
                {
                    if(epr_id.at(j) == a.ench.at(k).id)
                    {
                        if(edition == MCE::Java)    // 计算魔咒冲突花费（仅JE）
                            cost++;
                        u = true;
                        j = epr_id.count();
                        break;
                    }
                }
            }
            if(u)
                continue;

            // 计算魔咒合成花费
            int m = b.type == ItemType::weapon ? 0: 1;
            int p = getContentIndex(a.ench, b.ench.at(i), false);
            if(p > -1)  // 合并
            {

                if(edition == MCE::Java)
                    cost += eps.at(b.ench.at(i).id).multiplier[m] * ((a.ench.at(p).lvl == b.ench.at(i).lvl && a.ench.at(p).lvl < eps.at(a.ench.at(p).id).max_level) ? a.ench.at(p).lvl+1: std::max(a.ench.at(p).lvl, b.ench.at(i).lvl));
                else
                    cost += eps.at(b.ench.at(i).id).multiplier[m] * ((a.ench.at(p).lvl == b.ench.at(i).lvl && a.ench.at(p).lvl < eps.at(a.ench.at(p).id).max_level) ? 1: std::max(a.ench.at(p).lvl, b.ench.at(i).lvl) - a.ench.at(p).lvl);
            }
            else    // 直接添加
            {
                cost += eps.at(b.ench.at(i).id).multiplier[m] * b.ench.at(i).lvl;
            }
        }
        else
            continue;
    }

    return cost;
}

int Calculator::preForge(const _Item &it, bool strict)
{
    int cost = 0;
    if(strict)
        cost += penaltyCost(it.penalty);    // 计算累积惩罚

    for(int i = 0; i < it.ench.count(); i++) // 遍历 it
    {
        if(edition == MCE::Java)    // 计算魔咒冲突花费（仅JE）
        {
            QVector<int> epr_id = eps.at(it.ench.at(i).id).repulsion;
            for(int j = 0; j < epr_id.count(); j++) // 遍历 it 的 repulsion
            {
                for(int k = 0; k < output_item.ench.count(); k++) // 遍历 output_item 的 ench
                {
                    if(epr_id.at(j) == output_item.ench.at(k).id)
                    {
                        cost++;
                        j = epr_id.count();
                        break;
                    }
                }
            }
        }

        cost += eps.at(it.ench.at(i).id).multiplier[it.type == ItemType::weapon ? 0: 1] * it.ench.at(i).lvl;    // 计算魔咒合成花费
    }
    return cost;
}

_Item Calculator::forge(const _Item &a, const _Item &b)
{
    _Item it = a;
    for(int i = 0; i < b.ench.count(); i++)
    {
        if(checkCombinable(it, b.ench.at(i)))
        {
            int p = getContentIndex(it.ench, b.ench.at(i), false);
            if(p > -1)
            {
                if(it.ench.at(p).lvl < eps.at(it.ench.at(p).id).max_level && it.ench.at(p).lvl == b.ench.at(i).lvl)
                    it.ench[p].lvl++;
                else
                    it.ench[p].lvl = std::max(it.ench.at(p).lvl, b.ench.at(i).lvl);
            }
            else
            {
                it.ench.append(b.ench.at(i));
            }
        }
    }
    it.penalty = std::max(a.penalty, b.penalty) + 1;
    return it;
}

_FlowStep Calculator::forge(_Item *a, const _Item &b)
{
    _FlowStep fs;
    fs.a = *a;
    fs.b = b;
    fs.levelCost = preForge(*a, b);
    fs.pointCost = levelToPoint(fs.levelCost);
    for(int i = 0; i < b.ench.count(); i++)
    {
        if(checkCombinable(*a, b.ench.at(i)))
        {
            int p = getContentIndex(a->ench, b.ench.at(i), false);
            if(p > -1)
            {
                if(a->ench.at(p).lvl < eps.at(a->ench.at(p).id).max_level && a->ench.at(p).lvl == b.ench.at(i).lvl)
                    a->ench[p].lvl++;
                else
                    a->ench[p].lvl = std::max(a->ench.at(p).lvl, b.ench.at(i).lvl);
            }
            else
            {
                a->ench.append(b.ench.at(i));
            }
        }
    }
    a->penalty = std::max(a->penalty, b.penalty) + 1;
    return fs;
}


// 排序函数
void Calculator::_sort_1(int left, int right)
{
    if(left > right)
        return;

    int temp = integratingDegree(item_pool.at(left));
    int i = left;
    int j = right;

    while(i != j)
    {
        while(integratingDegree(item_pool.at(j)) >= temp && j > i)
            j--;
        while(integratingDegree(item_pool.at(i)) <= temp && j > i)
            i++;
        if(i < j)
            item_pool.swapItemsAt(i, j);
    }
    item_pool.swapItemsAt(left, i);

    _sort_1(left, i-1);
    _sort_1(i+1, right);
}

void Calculator::sortPool(QVector<_Item> *its, int left, int right)
{
    if(left > right)
        return;

    int temp = preForge(its->at(left));
    int i = left;
    int j = right;

    while(i != j)
    {
        while(preForge(its->at(j)) <= temp && j > i)
            j--;
        while(preForge(its->at(i)) >= temp && j > i)
            i++;
        if(i < j)
            its->swapItemsAt(i, j);
    }
    its->swapItemsAt(left, i);

    sortPool(its, left, i-1);
    sortPool(its, i+1, right);
}


// 算法函数
void Calculator::alg_0()
{
    // 建立计算模型
    int mP = 0;
    QVector<QVector<_Item>> tm_item_triangle, item_triangle;

    for(int i = 0; i < item_pool.count(); i++)
        mP = std::max(mP, item_pool.at(i).penalty);

    tm_item_triangle.fill(QVector<_Item>({}), mP+1);

    tm_item_triangle[0].append(input_item);
    for(int i = 0; i < item_pool.count(); i++)
        tm_item_triangle[item_pool.at(i).penalty].append(item_pool.at(i));

    for(int i = 0; i < tm_item_triangle.count(); i++)
    {
        while(i > item_triangle.count()-1)
            item_triangle.append(QVector<_Item>({}));

        int n = tm_item_triangle.at(i).count();
        sortPool(&tm_item_triangle[i], 0, n-1);
        item_triangle[i].fill(_Item(), n);

        for(int j = 0; j < n; j++)
        {
            if(tm_item_triangle.at(i).at(j).type == ItemType::weapon)
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
            _Item a = tm_item_triangle[i].takeFirst();
            _Item b = tm_item_triangle[i].takeFirst();
            _Item tm = forge(a, b);

            while(tm.penalty > tm_item_triangle.count()-1 || i+1 > tm_item_triangle.count()-1)
                tm_item_triangle.append(QVector<_Item>({}));

            if(tm.type == ItemType::weapon)
                tm_item_triangle[i+1].append(tm);
            else
                tm_item_triangle[tm.penalty].append(tm);
        }
        if(tm_item_triangle.at(i).count() > 0)
        {
            while(i >= tm_item_triangle.count()-1)
                tm_item_triangle.append(QVector<_Item>({}));

            tm_item_triangle[i+1].append(tm_item_triangle[i].takeFirst());
        }
    }

    // 打印模型运算结果
    for(int i = 0; i < item_triangle.count(); i++)
    {
        for(int j = 0; j < item_triangle.at(i).count(); j++)
        {
            QString str = "["+QString::number(i+1)+"|"+QString::number(j+1)+"]\n" + formatToCore(item_triangle.at(i).at(j)).name + " P:"+QString::number(item_triangle.at(i).at(j).penalty) + ", C:"+QString::number(preForge(item_triangle.at(i).at(j))) + ", R:"+QString::number(duplicationSeries(j)) + "\n";
            for(int k = 0; k < item_triangle.at(i).at(j).ench.count(); k++)
            {
                str += " - ["+QString::number(k+1)+"]" + core->ench_table.at(item_triangle.at(i).at(j).ench.at(k).id).name + " " + QString::number(item_triangle.at(i).at(j).ench.at(k).lvl);
                if(k < item_triangle.at(i).at(j).ench.count()-1)
                    str += "\n";
            }
            mdb.msg(str);
        }
        mdb.msg("-------------------------\n");
    }

    for(int i = 0; i < item_triangle.count(); i++)
    {
        while(item_triangle.at(i).count() > 1)
        {
            _Item tm = item_triangle[i].takeFirst();
            flow.append(forge(&tm, item_triangle[i].takeFirst()));
        }
    }

}

void Calculator::alg_1()
{

}

void Calculator::alg_2()
{

}


void Calculator::run()
{
    mdb.msg("开始计算.");
    start_t = QDateTime::currentDateTime();
    optimizePool();

    alg_0();

    output_item = forge(flow.last().a, flow.last().b);
    end_t = QDateTime::currentDateTime();

    core->summary.step_count = flow.count();
    core->summary.level_cost = 0;
    core->summary.point_cost = 0;
    core->summary.time_cost = start_t.msecsTo(end_t);
    core->summary.output_item = formatToCore(output_item);
    core->flow.clear();
    for(int i = 0; i < flow.count(); i++)
    {
        core->summary.level_cost += flow.at(i).levelCost;
        core->summary.point_cost += flow.at(i).pointCost;
        core->flow.append(formatToCore(flow.at(i)));
    }
}

