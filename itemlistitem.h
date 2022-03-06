#ifndef ITEMLISTITEM_H
#define ITEMLISTITEM_H

#include <QListWidgetItem>

class ItemListItem : public QListWidgetItem
{
    Q_OBJECT
public:
    explicit ItemListItem(QListWidget *parent = nullptr);

signals:

};

#endif // ITEMLISTITEM_H
