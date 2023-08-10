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

bool Calculator::setConfig(ICM icm, MCE mce, Group g, Item target, ItemPool ip, QString alg, ALGCFG algcfg)
{
    edition = mce;

    item_pool.clear();
    foreach (auto &i, ip)
        item_pool.append(i);

    group.name = g.name;
    group.max_durability = g.max_durability;
    foreach (auto &e, g.enchantments)
        group.enchantments.append(_Ench({e.name, mce, 0}).id);

    target_item = target;

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
    emit start(group, target_item, item_pool, edition, alg_config);
    return true;
}

void Calculator::getResult(FlowStack &fs)
{
    fs.clear();
    foreach (auto &_flow, flows)
    {
        StepList steps;
        foreach (auto &_step, _flow.steps)
            steps.append(_step);
        fs.append({_flow.summary, steps});
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
            int em = CTEp.at(need.id).poor_max_lvl;

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

void Calculator::receiveResult(_FlowStack fs)
{
    disconnect(algorithm);
    flows = fs;

    algorithm->deleteLater();
    algorithm = nullptr;
    thread->deleteLater();
    thread = nullptr;

    emit finished();
}
