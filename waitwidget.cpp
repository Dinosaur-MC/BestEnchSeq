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

void WaitWidget::Done()
{
    //...
    close();
}

