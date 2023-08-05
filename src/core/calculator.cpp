#include "calculator.h"
#include <QThread>

Calculator::Calculator(QObject *parent) : QObject{parent}
{
}

Calculator::~Calculator()
{
    if (thread != nullptr)
        delete thread;
}

void Calculator::startCalc(QSet<EnchData> ench_set, Item target, MCE mce, ALG alg_index, ALGCFG cfg, ICM icm, ItemPool *ip)
{
    if (icm == ICM::Normal)
    {
        item_pool.clear();
    }
    else if (icm == ICM::Poor)
    {
        item_pool.clear();
    }
    else if (icm == ICM::Advanced && ip != nullptr)
    {
        item_pool.clear();
        foreach (Item it, *ip)
            item_pool.append(it);
    }
    else
        return;

    target_item = target;
    QSet<int> _ench_set;
    foreach (auto &ed, ench_set)
    {
        Ench e;
        e.name = ed.name;
        e.mce = ed.editions;
        _ench_set.insert(((_Ench)e).id);
    }

    if (alg_index == ALG::Intelligent)
    {
    }
    else if (alg_index == ALG::Enumerating)
    {
    }
    else if (alg_index == ALG::External)
    {
    }
    else
        return;

    connect(this, &Calculator::start, alg, &Algorithm::start);
    connect(this, &Calculator::stepForwarded, alg, &Algorithm::stepForwarded);
    connect(alg, &Algorithm::finished, this, &Calculator::receiveResult);

    thread = new QThread;
    alg->moveToThread(thread);
    emit start(_ench_set, target, item_pool, mce, cfg);
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

void Calculator::receiveResult(Summary s, _FlowStack fs)
{
    disconnect(alg);

    summary = s;
    flows = fs;

    thread->deleteLater();
    thread = nullptr;
    emit finished();
}
