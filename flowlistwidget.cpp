#include "flowlistwidget.h"
#include "ui_flowlistwidget.h"

FlowListWidget::FlowListWidget(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::FlowListWidget)
{
    ui->setupUi(this);
}

FlowListWidget::~FlowListWidget()
{
    delete ui;
}
