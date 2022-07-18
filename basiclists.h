#ifndef BASICLISTS_H
#define BASICLISTS_H


#include <QListWidget>
#include <QComboBox>
#include "basicvariable.h"


class ListWidget_Ench : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget_Ench(QWidget *parent = nullptr);
    ~ListWidget_Ench();

    Ench item(int);
    void pushItem(Ench);

    int checkedItemCount();
    void refresh();

signals:
    void itemStateChanged();

};

class ListWidget_Item : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget_Item(QWidget *parent = nullptr);
    ~ListWidget_Item();

    Item item(int);
    void pushItem(Item);

    int checkedItemCount();
    void refresh();

signals:
    void itemStateChanged();

};

class ListWidget_FlowStep : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget_FlowStep(QWidget *parent = nullptr);
    ~ListWidget_FlowStep();

    FlowStep item(int);
    void pushItem(FlowStep);

    int checkedItemCount();
    void refresh();

signals:
    void itemStateChanged();

};


namespace Ui1 {
class ListItemWidget_Ench;
};
class ListItemWidget_Ench : public QWidget
{
    Q_OBJECT

public:
    explicit ListItemWidget_Ench(QWidget *parent = nullptr);
    ~ListItemWidget_Ench();

    Ench item(int);
    void setItem(Ench);

    void setEnable(bool);
    bool isEnabled();
    void setCheck(bool);
    bool isChecked();

    void setHideDisabled(bool);

private:
    Ench this_item;
    Ui1::ListItemWidget_Ench *ui;

signals:
    void stateChanged();
    void valueChanged();

};

namespace Ui2 {
class ListItemWidget_Item;
};
class ListItemWidget_Item : public QWidget
{
    Q_OBJECT

public:
    explicit ListItemWidget_Item(QWidget *parent = nullptr);
    ~ListItemWidget_Item();

    Item item(int);
    void setItem(Item);

    void setEnable(bool);
    bool isEnabled();
    void setCheck(bool);
    bool isChecked();

    void setHideDisabled(bool);

private:
    Item this_item;
    Ui2::ListItemWidget_Item *ui;

signals:
    void stateChanged();
    void valueChanged();

};

namespace Ui3 {
class ListItemWidget_FlowStep;
};
class ListItemWidget_FlowStep : public QWidget
{
    Q_OBJECT

public:
    explicit ListItemWidget_FlowStep(QWidget *parent = nullptr);
    ~ListItemWidget_FlowStep();

    FlowStep item(int);
    void setItem(FlowStep);

    void setEnable(bool);
    bool isEnabled();
    void setCheck(bool);
    bool isChecked();

    void setHideDisabled(bool);

private:
    FlowStep this_item;
    Ui3::ListItemWidget_FlowStep *ui;

signals:
    void stateChanged();
    void valueChanged();

};


class WeaponBox : public QComboBox
{
    Q_OBJECT

public:
    WeaponBox();

signals:

};


#endif // BASICLISTS_H
