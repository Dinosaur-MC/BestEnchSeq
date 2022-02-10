#include "flowlistwidget.h"
#include <QScrollBar>

#include "qlistview.h"



using namespace std;

FlowListWidget::FlowListWidget(QWidget *parent)
    : QListWidget{parent}
{
    setResizeMode(QListView::Adjust);
    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void FlowListWidget::addStep(Step s, QIcon icon)
{
    QListWidgetItem *item = new QListWidgetItem();
    addItem(item);

    QString stepA = "";
    for(int i = 0; &s.tar.ench[i] != end(s.tar.ench) && s.tar.ench[i].name != ""; i++)
    {
        stepA += s.tar.ench[i].name + " " + Common::IntToRoman(s.tar.ench[i].lvl);
        if(&s.tar.ench[i+1] != end(s.tar.ench) && s.tar.ench[i+1].name != "")
            stepA += '\n';
    }
    QString stepB = "";
    for(int i = 0; &s.sac.ench[i] != end(s.sac.ench) && s.sac.ench[i].name != ""; i++)
    {
        stepB += s.sac.ench[i].name + " " + Common::IntToRoman(s.sac.ench[i].lvl);
        if(&s.sac.ench[i+1] != end(s.sac.ench) && s.sac.ench[i+1].name != "")
            stepB += '\n';
    }

    StepWidget *w = new StepWidget(this);

    if(s.tar.Durability == 0)
        w->setIconA(QIcon(":/icon/res/enchanted_book.png"));
    else
        w->setIconA(icon);
    w->setIconB(QIcon(":/icon/res/enchanted_book.png"));
    w->setTextA(stepA);
    w->setTextB(stepB);
    w->setCost(s.cost);

    w->setLayoutMode(QLayout::SetFixedSize);
    setItemWidget(item, w);
    item->setSizeHint(w->size());
    w->setLayoutMode(QLayout::SetDefaultConstraint);

    connect(this, &FlowListWidget::SizeChanged, w, [=](){
        int Width = width() - 2 - 2 * lineWidth();
        if(verticalScrollBar()->isVisible())
            Width -= verticalScrollBar()->width();
        if(Width != width())
            w->setFixedWidth(Width);
    });
    connect(this, &FlowListWidget::Refresh, w, &QWidget::close);
}

void FlowListWidget::refresh()
{
    emit Refresh();
}

void FlowListWidget::refreshSize()
{
    emit SizeChanged();
}

void FlowListWidget::resizeEvent(QResizeEvent *e)
{
    QAbstractItemView::resizeEvent(e);
    emit SizeChanged();
}
