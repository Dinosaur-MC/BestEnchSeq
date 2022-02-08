#ifndef ENCHANTMENTLIST_H
#define ENCHANTMENTLIST_H

#include <QListWidget>
#include "common.h"

class EnchantmentList : public QListWidget
{
    Q_OBJECT
public:
    explicit EnchantmentList(QWidget *parent = nullptr);
    ~EnchantmentList();

    void refreshEnch();
    void setMode(int a);
    Ench *getEnch();
    void clearEnch();

private:
    Ench Existed[64];
    int Listmode = 0;

signals:

};

#endif // ENCHANTMENTLIST_H
