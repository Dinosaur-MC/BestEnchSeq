#ifndef BASICOPERATOR_H
#define BASICOPERATOR_H


#include <QObject>
#include <QString>
#include <QVector>
#include "basicdata.h"
#include "basiclists.h"
#include "basicvariable.h"


class FileOperator : public QObject
{
public:
    explicit FileOperator(QObject *parent = nullptr);

    void saveConfig(Config config, const QVector<Option> *opt);
    void saveWeaponTable(QVector<raw_Weapon>);
    void saveEnchantmentTable(QVector<raw_EnchPlus>);

    void loadConfig(Config *config, const QVector<Option> *opt);
    void loadWeaponTable(QVector<raw_Weapon> *);
    void loadEnchantmentTable(QVector<raw_EnchPlus> *);

    void saveExport(const Config, const QVector<raw_EnchPlus>, const Summary, const QVector<FlowStep>);

signals:

};


class Anvil
{
public:
    explicit Anvil(MCE, PFADDN, const QVector<EnchPlus> *);

    bool checkRepulsed(const Ench, const Ench);
    bool checkRepulsed(const Ench, const Item);
    int preforge(const Item, Item);
    int forge(int, int);
    Item forge(const Item, Item);

    int compare(const Item, Item);
    FlowStep combine(const Item, Item);

private:
    MCE edition;
    PFADDN addition;
    const QVector<EnchPlus> *eps;

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


void deliverID(QVector<raw_EnchPlus> *reps, QVector<raw_Weapon> *rwps, QVector<EnchPlus> *eps, QVector<Weapon> *wps);

QString numToRoman(int);


#endif // BASICOPERATOR_H
