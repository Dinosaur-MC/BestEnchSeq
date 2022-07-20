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

    void saveConfig();
    void saveWeaponTable(QVector<raw_Weapon>);
    void saveEnchantmentTable(QVector<raw_EnchPlus>);

    void loadConfig();
    void loadWeaponTable(QVector<raw_Weapon> *);
    void loadEnchantmentTable(QVector<raw_EnchPlus> *);

    void saveExport(Summary summary, QVector<FlowStep>);

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


bool checkRepulsed(Ench, Ench);
bool checkRepulsed(Ench, Item);
int* preforge(Item, Item, MCE);
int forge(int, int);
Item forge(Item, Item);


#endif // BASICOPERATOR_H
