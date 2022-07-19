#ifndef BASICOPERATOR_H
#define BASICOPERATOR_H


#include <QObject>
#include <QVector>
#include "basicdata.h"
#include "basicvariable.h"
#include "basiclists.h"


class FileOperator : public QObject
{
public:
    explicit FileOperator(QObject *parent = nullptr);

    static void saveConfig();
    static void saveWeaponTable(QVector<raw_Weapon>);
    static void saveEnchantmentTable(QVector<raw_EnchPlus>);

    static void loadConfig();
    static void loadWeaponTable(QVector<raw_Weapon> *);
    static void loadEnchantmentTable(QVector<raw_EnchPlus> *);

    void saveExport(ListItemWidget_FlowStep *);

signals:

};


class EnchFilter
{
public:
    explicit EnchFilter();

};

class ItemFilter
{
public:
    explicit ItemFilter();

};


void deliverID();


bool checkRepulsed(Ench, Item);
int* preforge(Item, Item, MCE);
int forge(int, int);
Item forge(Item, Item);


#endif // BASICOPERATOR_H
