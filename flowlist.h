#ifndef FLOWLIST_H
#define FLOWLIST_H

#include <QListWidget>
#include "datamanager.h"
#include "flowlistwidget.h"

class FlowList : public QListWidget
{
    Q_OBJECT
public:
    explicit FlowList(QWidget *parent = nullptr);

    void refresh();

private:

protected:
//    void resizeEvent(QResizeEvent *event);

signals:
//    void resized();
};

#endif // FLOWLIST_H
