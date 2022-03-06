#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QListWidget>
#include "basic.h"

class ItemList : public QListWidget
{
    Q_OBJECT
public:
    explicit ItemList(QWidget *parent = nullptr);

    bool setWeapon(QString name);
    QString Weapon();
    int enchCount();
    void getAvailable();
    void refresh();
    void uploadList();
    void checkRepulsion();

    void itemClickedOperation();
    void itemDoubleClickedOperation();

private:
    int current_weapon_index;
    Item this_item;

signals:
    void resized();

protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // ITEMLIST_H
