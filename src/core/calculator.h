#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QThread>
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

    bool setConfig(MCE mce, Item target, ItemPool ip, QSet<EnchData> ench_g, ICM icm, QString alg, ALGCFG algcfg);
    bool startCalc();
    void getResult(Summary &s, FlowStack &fs);

private:
    bool generateItemPool(ICM icm, _Item target, _ItemPool &ip);
    bool initializeAlgorithm();
    void receiveResult(Summary s, _FlowStack fs);

    MCE edition;
    _Item target_item;
    _ItemPool item_pool;
    QSet<int> ench_group;
    QString algorithm_name;
    ALGCFG alg_config;

    _FlowStack flows;
    Summary summary;

    QThread *thread;
    Algorithm *algorithm;

    uint current_step;
    uint step_count;

signals:
    void start(QSet<int> es, _Item tar, _ItemPool ip, MCE mce, ALGCFG cfg);
    void stepForwarded(uint current, uint total);
    void finished();
};

#endif // CALCULATOR_H
