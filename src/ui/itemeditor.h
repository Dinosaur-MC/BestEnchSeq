#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include "core/core.h"
#include <QDialog>

namespace Ui
{
    class ItemEditor;
}

class ItemEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ItemEditor(const Group &g, QWidget *parent = nullptr);
    ~ItemEditor();

    void setGroup(const Group &g);
    void setItem(Item item);
    void refresh();
    ItemPool getItem();

private:
    Group group;
    ItemType type;
    Item _item;

    Ui::ItemEditor *ui;

signals:
};


#endif // ITEMEDITOR_H
