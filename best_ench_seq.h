#ifndef BEST_ENCH_SEQ_H
#define BEST_ENCH_SEQ_H

#include <QMainWindow>
#include "common.h"
#include "enchantmentlist.h"
#include "flowlistwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BestEnchSeq; }
QT_END_NAMESPACE

class BestEnchSeq : public QMainWindow
{
    Q_OBJECT

public:
    BestEnchSeq(QWidget *parent = nullptr);
    ~BestEnchSeq();

    void loadEnchantmentTable();
    void calculate();


    int minCost(Item target, Item *items, int len);
    void listMinSet(Step *list, int cap, Item target, Item *items, int len, int min_cost);
    int minPenalty(Step *list, int len);
    void listMinPenalty(Step *list, int len, int min_penalty);
    Step minSumLevel(Step *list, int len);

    int preForgeItem(Item target, Item sacrifice);
    Item forgeItem(Item target, Item sacrifice);

private:
    Ui::BestEnchSeq *ui;

signals:
    void calc();
};

#endif // BEST_ENCH_SEQ_H
