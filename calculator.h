#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include "basic.h"

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

    void Alg_DifficultFirst();
    void Alg_Greedy();
    void Alg_Enumeration();


    Step preForge(Item A, Item B, int mode);
    Step Forge(Item A, Item B, int mode);
    void sort();

private:
    int A_list_l, B_list_l, C_list_l, flow_l;
    Item *A_list, *B_list, *C_list;
    Step tmFlow, *flow;

    int cost_list_l;
    struct COSTL {int p; int c;}*cost_list;

signals:

};

#endif // CALCULATOR_H
