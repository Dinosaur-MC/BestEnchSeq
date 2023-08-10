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

    bool setConfig(ICM icm, MCE mce, Group g, Item target, ItemPool ip, QString alg, ALGCFG algcfg);
    bool startCalc();
    void getResult(FlowStack &fs);

private:
    bool generateItemPool(ICM icm, _Item target, _ItemPool &ip);
    bool initializeAlgorithm();
    void receiveResult(_FlowStack fs);

    MCE edition;
    _Item target_item;
    _ItemPool item_pool;
    _Group group;
    QString algorithm_name;
    ALGCFG alg_config;

    _FlowStack flows;

    QThread *thread;
    Algorithm *algorithm;

    uint current_step;
    uint step_count;

signals:
    void start(_Group g, _Item tar, _ItemPool ip, MCE mce, ALGCFG cfg);
    void stepForwarded(uint current, uint total);
    void finished();
};

#endif // CALCULATOR_H
