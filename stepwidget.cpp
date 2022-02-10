#include "stepwidget.h"
#include "ui_stepwidget.h"

StepWidget::StepWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

StepWidget::~StepWidget()
{
    delete ui;
}

void StepWidget::setIconA(QIcon icon)
{
    ui->tbA->setIcon(icon);
}

void StepWidget::setIconB(QIcon icon)
{
    ui->tbB->setIcon(icon);
}

void StepWidget::setTextA(QString str)
{
    ui->labelA->setText(str);
}

void StepWidget::setTextB(QString str)
{
    ui->labelB->setText(str);
}

void StepWidget::setCost(int cost)
{
    ui->label->setText(QString::number(cost));
}

void StepWidget::setLayoutMode(QLayout::SizeConstraint a)
{
    ui->BasisLayout->setSizeConstraint(a);
}

void StepWidget::AutoWidth(int w)
{
    setFixedWidth(w);
}

