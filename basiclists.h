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

    Ench item();
    void setItem(Ench, EnchPro);

    void setCheck(bool);
    bool isChecked();

    void setCheckboxHidden(bool);
    void setSpinboxHidden(bool);

private:
    Ench this_item;
    Ui::ItemWidget_Ench *ui;



signals:
    void stateChanged();

};


class ItemWidget_Item : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget_Item(QWidget *parent = nullptr);
    ~ItemWidget_Item();

    Item item();
    void setItem(Item, ItemPro);

    void setCheck(bool);
    bool isChecked();

    void setCheckboxHidden(bool);
    void setSpinboxHidden(bool);

private:
    Item this_item;
    Ui::ItemWidget_Item *ui;

signals:
    void stateChanged();

};


class ItemWidget_FlowStep : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget_FlowStep(QWidget *parent = nullptr);
    ~ItemWidget_FlowStep();

    FlowStep item();
    void setItem(FlowStep, FlowStepPro);

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
    void pushItem(Ench, EnchPro); // 追加物品

    int checkedItemCount(); // 被选中的项数
    QVector<Ench> getCheckedItem();  // 获取被选中的项
    void reload(const QVector<Ench> *, const QVector<EnchPro> *);  // 加载（重载）列表

signals:
    void itemStateChanged();    // 存在项目选中状态改变

};


class ListWidget_Item : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget_Item(QWidget *parent = nullptr);

    ItemWidget_Item* getItem(int);   // 获取指定行的项目值
    void pushItem(Item, ItemPro); // 追加物品

    int checkedItemCount(); // 被选中的项数
    QVector<Item> getCheckedItem();  // 获取被选中的项
    void reload(const QVector<Item> *, const QVector<ItemPro> *);  // 加载（重载）列表

signals:
    void itemStateChanged();    // 存在项目选中状态改变

};


class ListWidget_FlowStep : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget_FlowStep(QWidget *parent = nullptr);

    ItemWidget_FlowStep* getItem(int);   // 获取指定行的项目值
    void pushItem(FlowStep, FlowStepPro); // 追加物品

    void reload(const QVector<FlowStep> *, const QVector<FlowStepPro> *);  // 加载（重载）列表

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
