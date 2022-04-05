#ifndef ENCHLISTWIDGET_H
#define ENCHLISTWIDGET_H

#include <QWidget>
#include "datamanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EnchListWidget; }
QT_END_NAMESPACE

class EnchListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EnchListWidget(QWidget *parent = nullptr);
    ~EnchListWidget();

    void setTestList(Ench *ep, int ep_l);
    void setEnch(EnchPlus ench, int m = 0);   //设置魔咒
    void setEnchLevel(int lvl);  //设置魔咒等级
    Ench getEnch();    //获取魔咒
    void setChecked(bool);  //设置选中状态
    bool isChecked();   //获取选中状态
    void checkRepulsion();  //检查冲突魔咒并处理

private:
    Ui::EnchListWidget *ui;
    int current_row;
    int ench_list_l;
    Ench *ench_list;

signals:
    void stateChanged();    //勾选状态已改变
    void valueChanged();    //被勾选的项等级值已改变
};

#endif // ENCHLISTWIDGET_H
