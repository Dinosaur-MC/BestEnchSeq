#ifndef SELECTOR_H
#define SELECTOR_H

#include <QDialog>
#include "common.h"

namespace Ui {
class Selector;
}

class Selector : public QDialog
{
    Q_OBJECT

public:
    explicit Selector(QDialog *parent = nullptr);
    ~Selector();

    void setup(Ench iCurrent, Ench *iExisted, int m);
    void refresh();
    Ench getSelectedEnch();
    bool isEmpty();
    int availableCount();

private:
    Ui::Selector *ui;

    Ench Avoid[64], Avoid2[64], Available[64], Current;

};

#endif // SELECTOR_H
