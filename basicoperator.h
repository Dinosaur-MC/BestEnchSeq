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
    explicit Anvil(MCE *, unsigned short *, const QVector<EnchPlus> *);
    explicit Anvil(const QVector<EnchPlus> *);
    ~Anvil();

    bool checkRepulsed(const Ench, const Ench);
    bool checkRepulsed(const Ench, const Item);
    int *preforge(const Item, Item);
    int forge(int, int);
    Item forge(const Item, Item);

    int compare(const Item, Item);
    FlowStep combine(const Item, Item);

private:
    MCE *edition;
    unsigned short *addition;
    const QVector<EnchPlus> *eps;

    bool simple_mode;

};


class EnchFilter
{
public:
    explicit EnchFilter(const MCE *, const QVector<Weapon> *, const QVector<EnchPlus> *);

    void setWeapon(const Weapon *);
    void setBase(const QVector<Ench> *es);

    QVector<Ench> getEnchSet();

private:
    const MCE *edition;
    const QVector<Weapon> *w_table;
    const QVector<EnchPlus> *e_table;

    const Weapon *weapon;
    const QVector<Ench> *base_set;

};

class ItemFilter
{
public:
    explicit ItemFilter();

};


class Transformer
{
public:
    Transformer(const QVector<raw_Weapon> *, const QVector<raw_EnchPlus> *);

    ItemPro toItemPro(const Item*);
    FlowStepPro toFlowStepPro(const FlowStep*);

private:
    const QVector<raw_Weapon> *rwps;
    const QVector<raw_EnchPlus> *reps;

};


void deliverID(const QVector<raw_Weapon> *rwps, const QVector<raw_EnchPlus> *reps, QVector<Weapon> *wps, QVector<EnchPlus> *eps);
bool checkRedefinition(const QVector<raw_EnchPlus> *);
bool checkRedefinition(const QVector<raw_Weapon> *);
QString numToRoman(int);


#endif // BASICOPERATOR_H
