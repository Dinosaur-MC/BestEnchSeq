#include "waitwidget.h"
#include "ui_waitwidget.h"

WaitWidget::WaitWidget(QWidget *parent):
    QDialog{parent},
    ui(new Ui::WaitWidget)
{
    ui->setupUi(this);
}

WaitWidget::~WaitWidget()
{
    delete ui;
}

void WaitWidgetThread::run()
{
    WaitWidget *ww = new WaitWidget();

    ww->exec();
    emit isDone();
}

