#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include <QDialog>
#include "base.h"

QT_BEGIN_NAMESPACE
namespace Ui {class ItemEditor;}
QT_END_NAMESPACE

class ItemEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ItemEditor(QWidget *parent = nullptr);
    ~ItemEditor();

    void setItem(Item item);
    Item item();

    static Ench *resizeEnchList(int len);
    static int enchlist_l;
    static Ench *enchlist;

private:
    Ui::ItemEditor *ui;
    Item this_item;

signals:

};

#endif // ITEMEDITOR_H
