#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include "basic.h"
#include "itempool.h"

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

    void Alg_DifficultyFirst();
    void Alg_Greedy();
    void Alg_Enumeration();

    void uploadData();

private:
    int flow_l, flow_step;
    Step *flow;
    ForgeMode additional_mode;
    ItemPool pool;

signals:

};

#endif // CALCULATOR_H
