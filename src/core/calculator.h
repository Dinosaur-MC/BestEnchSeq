#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "core.h"
#include "algorithm.h"

struct TraceLog
{
};

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);
    ~Calculator();

    void startCalc(QSet<EnchData> ench_set, Item target, MCE mce, ALG alg_index, ALGCFG cfg, ICM icm, ItemPool *ip = nullptr);
    void getResult(Summary &s, FlowStack &fs);

private:
    void receiveResult(Summary s, _FlowStack fs);

    _Item target_item;
    _ItemPool item_pool;
    _FlowStack flows;
    Summary summary;

    QThread *thread;
    Algorithm *alg;

    uint current_step;
    uint step_count;

signals:
    void start(QSet<int> es, _Item tar, _ItemPool ip, MCE mce, ALGCFG cfg);
    void stepForwarded(uint current, uint total);
    void finished();
};

#endif // CALCULATOR_H
