#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include "datamanager.h"
#include "itempool.h"

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

    void preparation(); //预处理

    void Alg_GlobeAverage();    //核心算法
    void Alg_DifficultyFirst();
    void Alg_Greedy();
    void Alg_Enumeration();
    void Alg_SimpleEnumeration();

    void uploadData();  //数据上传

private:
    int flow_l, flow_step;
    Step *flow;
    ForgeMode additional_mode;
    ItemPool pool;

signals:

};

#endif // CALCULATOR_H
