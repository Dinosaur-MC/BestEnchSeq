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


void FlowListWidget::setStep(Step s)
{
    int p = Basic::searchWeapon(s.a.name);
    if(p != -1)
        ui->iconA->setIcon(Basic::weapon[p].icon);
    else
        ui->iconA->setIcon(QIcon(":/icon/res/enchanted_book.png"));

    p = Basic::searchWeapon(s.b.name);
    if(p != -1)
        ui->iconB->setIcon(Basic::weapon[p].icon);
    else
        ui->iconB->setIcon(QIcon(":/icon/res/enchanted_book.png"));

    QString str = "";
    for(int i = 0; i < INIT_LENGTH  && !s.a.ench[i].name.isEmpty(); i++)
    {
        if(i != 0)
            str+= '\n';
        str += s.a.ench[i].name;
        str += "  ";
        str += Basic::IntToRoman(s.a.ench[i].lvl);
    }
    ui->enchA->setText(str);

    str = "";
    for(int i = 0; i < INIT_LENGTH  && !s.b.ench[i].name.isEmpty(); i++)
    {
        if(i != 0)
            str+= '\n';
        str += s.b.ench[i].name;
        str += "  ";
        str += Basic::IntToRoman(s.b.ench[i].lvl);
    }
    ui->enchB->setText(str);

    ui->cost->setText(QString::number(s.cost));
}

