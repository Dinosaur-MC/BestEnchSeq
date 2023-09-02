#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QThread>
#include "core.h"
#include "algorithm.h"

struct TraceLog
{
};

enum class CalcState {Ready, Running, Paused, Stopped};

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
    void receiveResult(_FlowStack fs, _ItemPool unused);

    MCE edition;
    _Group group;
    _Item target_item;
    _ItemPool item_pool;
    _ItemPool unused_item;

    QString algorithm_name;
    ALGCFG alg_config;
    QThread *thread;
    Algorithm *algorithm;

    uint current_step;
    uint step_count;
    _FlowStack flows;

    CalcState state;

signals:
    void start(_Group g, _Item tar, _ItemPool ip, MCE mce, ALGCFG cfg);
    void stepForwarded(uint current, uint total);
    void finished();
};

#endif // CALCULATOR_H
