#ifndef BASICLISTS_H
#define BASICLISTS_H


#include <QWidget>
#include <QComboBox>
#include <QListWidget>
#include "basicvariable.h"


// ItemWidget
namespace Ui {
    class ItemWidget_Ench;
    class ItemWidget_Item;
    class ItemWidget_FlowStep;
};


class ItemWidget_Ench : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget_Ench(QWidget *parent = nullptr);
    ~ItemWidget_Ench();

    EnchPro item();
    void setItem(EnchPro);

    void setCheck(bool);
    bool isChecked();

    void setCheckboxHidden(bool);
    void setSpinboxHidden(bool);

private:
    EnchPro this_item;
    Ui::ItemWidget_Ench *ui;



signals:
    void stateChanged();
//    void valueChanged();

};


class ItemWidget_Item : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget_Item(QWidget *parent = nullptr);
    ~ItemWidget_Item();

    Item item();
    void setItem(ItemPro);

    void setCheck(bool);
    bool isChecked();

    void setCheckboxHidden(bool);
    void setSpinboxHidden(bool);

private:
    Item this_item;
    Ui::ItemWidget_Item *ui;

signals:
    void stateChanged();
//    void valueChanged();

};


class ItemWidget_FlowStep : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget_FlowStep(QWidget *parent = nullptr);
    ~ItemWidget_FlowStep();

    FlowStep item();
    void setItem(FlowStepPro);

private:
    FlowStep this_item;
    Ui::ItemWidget_FlowStep *ui;

signals:

};




// ListWidget
class ListWidget_Ench : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget_Ench(QWidget *parent = nullptr);

    ItemWidget_Ench* getItem(int);   // 获取指定行的项目值
    void pushItem(EnchPro); // 追加物品

    int checkedItemCount(); // 被选中的项数
    void reload(QVector<EnchPro>);  // 加载（重载）列表

signals:
    void itemStateChanged();    // 存在项目选中状态改变

};


class ListWidget_Item : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget_Item(QWidget *parent = nullptr);

    ItemWidget_Item* getItem(int);   // 获取指定行的项目值
    void pushItem(ItemPro); // 追加物品

    int checkedItemCount(); // 被选中的项数
    void reload(QVector<ItemPro>);  // 加载（重载）列表

signals:
    void itemStateChanged();    // 存在项目选中状态改变

};


class ListWidget_FlowStep : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget_FlowStep(QWidget *parent = nullptr);

    ItemWidget_FlowStep* getItem(int);   // 获取指定行的项目值
    void pushItem(FlowStepPro); // 追加物品

    void reload(QVector<FlowStepPro>);  // 加载（重载）列表

signals:
    void itemStateChanged();    // 存在项目选中状态改变

};




class WeaponBox : public QComboBox
{
    Q_OBJECT

public:
    WeaponBox(QWidget *parent = nullptr);

    void addWeapon(raw_Weapon);
    void reload(QVector<raw_Weapon>);

    raw_Weapon currentWeapon();

signals:

};


#endif // BASICLISTS_H
