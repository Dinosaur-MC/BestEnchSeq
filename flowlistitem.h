#ifndef FLOWLISTITEM_H
#define FLOWLISTITEM_H

#include <QListWidgetItem>

class FlowListItem : public QListWidgetItem
{
    Q_OBJECT
public:
    explicit FlowListItem(QListWidget *parent = nullptr);

signals:

};

#endif // FLOWLISTITEM_H
