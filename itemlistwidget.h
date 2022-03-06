#ifndef ITEMLISTWIDGET_H
#define ITEMLISTWIDGET_H

#include <QWidget>
#include <QLayout>
#include "basic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ItemListWidget; }
QT_END_NAMESPACE

class ItemListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ItemListWidget(QWidget *parent = nullptr);
    ~ItemListWidget();

    void setItem(Item);
    Item item();
    void setEnch(EnchPlus ench);
    Ench ench();
    void setChecked(bool);
    bool isChecked();
    void checkRepulsion();
    void setLayoutMode(QLayout::SizeConstraint);

private:
    Ui::ItemListWidget *ui;
    Item this_item;

signals:
    void stateChanged();

};

#endif // ITEMLISTWIDGET_H
