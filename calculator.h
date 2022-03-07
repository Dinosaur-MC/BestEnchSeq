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
    Item Forge(Item A, Item B);
    void sort();

private:
    int A_item_el, B_list_l, C_list_l, sList_l, flow_l;
    Item A_item, *B_list, *C_list;
    Step *sList, *flow;

    int cost_list_l;
    struct COSTL {int p; int cost; int pen;}*cost_list;

    int mode;

signals:

};

#endif // CALCULATOR_H
