#ifndef ENCHLISTITEM_H
#define ENCHLISTITEM_H

#include <QListWidgetItem>

class EnchListItem : public QListWidgetItem
{
    Q_OBJECT
public:
    explicit EnchListItem(QListWidget *parent = nullptr);

signals:

};

#endif // ENCHLISTITEM_H
