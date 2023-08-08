#include "calculator.h"
#include "algmanager.h"
#include "algorithms/algenumerate.h"
#include "algorithms/alggreedy.h"
#include "algorithms/alghuffmantree.h"

Calculator::Calculator(QObject *parent) : QObject{parent}
{
}

Calculator::~Calculator()
{
    if (thread != nullptr)
        delete thread;
}

bool Calculator::setConfig(MCE mce, Item target, ItemPool ip, QSet<EnchData> ench_g, ICM icm, QString alg, ALGCFG algcfg)
{

    item_pool.clear();
    foreach (auto &i, ip)
        item_pool.append(i);

    target_item = target;
    foreach (auto &ed, ench_g)
    {
        Ench e;
        e.name = ed.name;
        e.mce = mce;
        ench_group.insert(((_Ench)e).id);
    }

    if (!generateItemPool(icm, target_item, item_pool))
        return false;

    algorithm_name = alg;
    alg_config = algcfg;

    return true;
}
bool Calculator::startCalc()
{
    if (!initializeAlgorithm())
        return false;

    thread = new QThread;
    algorithm->moveToThread(thread);
    emit start(ench_group, target_item, item_pool, edition, alg_config);
    return true;
}

void Calculator::getResult(Summary &s, FlowStack &fs)
{
    s = summary;

    fs.clear();
    foreach (QVector<_FlowStep> _flow, flows)
    {
        QVector<FlowStep> flow;
        foreach (_FlowStep _f, _flow)
            flow.append(_f);
        fs.append(flow);
    }
}

bool Calculator::generateItemPool(ICM icm, _Item target, _ItemPool &ip)
{
    if (icm == ICM::Normal)
    {
        auto &origin = ip.front().enchs;
        foreach (auto &ench, target.enchs)
        {
            _Item it = {ItemType::Book, {ench}, 0, 0};

            int p = Algorithm::findEnch(origin, ench, false);
            if (p != -1 && ench.lvl - origin[p].lvl == 1)
                it.enchs[0].lvl--;

            ip.append(it);
        }
    }
    else if (icm == ICM::Poor)
    {
        /* --------------------------------------------------
         * Origin(O), the Available(A), Need(N)
         * O + A --> N
         * A ∈ {lvl | lvl <= emlvl}, (N ∈ {lvl | lvl < mlvl})
         * -------------------------------------------------- */

        auto &origin = ip.front().enchs;
        foreach (auto &need, target.enchs)
        {
            _Item it = {ItemType::Book, {need}, 0, 0};

            int O = 0;
            int N = need.lvl;
            int em = global_p_ench_table.at(need.id).poor_max_lvl;

            int p = Algorithm::findEnch(origin, need, false);
            if (p != -1)
                O = origin[p].lvl;

            if (N <= em + 1)
            {
                if (N - O == 1)
                {
                    it.enchs[0].lvl = O;
                    ip.append(it);
                }
                else
                {
                    it.enchs[0].lvl = N;
                    ip.append(it);
                }
            }
            else
            {
                if (N - O == 1)
                {
                    it.enchs[0].lvl = em;
                    for (int i = 0; i < pow(2, O - em); i++)
                        ip.append(it);
                }
                else if (N - O == 2)
                {
                    it.enchs[0].lvl = em;
                    for (int i = 0; i < pow(2, N - em) - 1; i++)
                        ip.append(it);
                }
                else
                {
                    it.enchs[0].lvl = em;
                    for (int i = 0; i < pow(2, N - em); i++)
                        ip.append(it);
                }
            }
        }
    }
    else if (icm == ICM::Advanced)
        return Algorithm::checkConfig(target, ip);
    else
        return false;
    return true;
}
bool Calculator::initializeAlgorithm()
{
    if (algorithm_name == "$Internal_Enumerate_23w32a")
    {
        algorithm = new AlgEnumerate(this);
    }
    else if (algorithm_name == "$Internal_Greedy_23w32a")
    {
        algorithm = new AlgGreedy(this);
    }
    else if (algorithm_name == "$Internal_HuffmanTree_23w32a")
    {
        algorithm = new AlgHuffmanTree(this);
    }
    else
    {
        alg_manager.detectAlgorithm();
        algorithm = alg_manager.loadAlgorithm(algorithm_name);
    }

    connect(this, &Calculator::start, algorithm, &Algorithm::start);
    connect(this, &Calculator::stepForwarded, algorithm, &Algorithm::stepForwarded);
    connect(algorithm, &Algorithm::finished, this, &Calculator::receiveResult);

    return true;
}

void Calculator::receiveResult(Summary s, _FlowStack fs)
{
    disconnect(algorithm);

    summary = s;
    flows = fs;

    algorithm->deleteLater();
    algorithm = nullptr;
    thread->deleteLater();
    thread = nullptr;

    emit finished();
}
