#include "flowlist.h"

FlowList::FlowList(QWidget *parent)
    : QListWidget{parent}
{
    qDebug() << "[FlowList]";
}


void FlowList::refresh()
{
    qDebug() << "+ FlowList::refresh()";
    if(DM->flow_list_l < 1)
        return;
    clear();
    qDebug() << "Flowlist:" << DM->flow_list_l;
    for(int i = 0; i < DM->flow_list_l; i++)
    {
        FlowListWidget *w = new FlowListWidget(this);
        w->setStep(DM->flow_list[i]);
        w->show();
        QListWidgetItem *item = new QListWidgetItem(this,0);
        item->setSizeHint(QSize(w->sizeHint().width(), w->sizeHint().height()));
        this->setItemWidget(item,w);
    }
    qDebug() << "- FlowList::refresh()";
}

