#include "flowlist.h"
#include "flowlistwidget.h"

FlowList::FlowList(QWidget *parent)
    : QListWidget{parent}
{

}


void FlowList::refresh()
{
    if(Basic::flow_list[0].cost <= 0)
        return;
    clear();
    for(int i = 0; i < Basic::flow_list_l; i++)
    {
        FlowListWidget *w = new FlowListWidget(this);
        w->setStep(Basic::flow_list[i]);
        w->show();
        QListWidgetItem *item = new QListWidgetItem(this,0);
        item->setSizeHint(QSize(w->sizeHint().width(), w->sizeHint().height()));
        this->setItemWidget(item,w);
    }
}

