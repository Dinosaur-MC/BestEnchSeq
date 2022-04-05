#ifndef ITEMLISTWIDGET_H
#define ITEMLISTWIDGET_H

#include <QWidget>
#include <QLayout>
#include "datamanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ItemListWidget; }
QT_END_NAMESPACE

class ItemListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ItemListWidget(QWidget *parent = nullptr);
    ~ItemListWidget();

    void setItem(Item); //设置显示物品
    Item item();    //获取正在显示的物品
    void setChecked(bool);  //设置选中状态
    bool isChecked();   //获取选中状态

private:
    Ui::ItemListWidget *ui;
    Item this_item;

signals:
    void stateChanged();

};

#endif // ITEMLISTWIDGET_H
