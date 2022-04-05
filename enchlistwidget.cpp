#include "enchlistwidget.h"
#include "ui_enchlistwidget.h"

EnchListWidget::EnchListWidget(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::EnchListWidget)
{
    ui->setupUi(this);

    connect(ui->cb, &QCheckBox::stateChanged, this, &EnchListWidget::stateChanged);
    connect(ui->sb, &QSpinBox::valueChanged, this, [=](){
        if(isChecked())
            emit valueChanged();
    });
}

EnchListWidget::~EnchListWidget()
{
    delete ui;
}

void EnchListWidget::setTestList(Ench *ep, int ep_l)
{
    ench_list_l = ep_l;
    ench_list = ep;
}

void EnchListWidget::setEnch(EnchPlus ench, int m)    //设置魔咒
{
    ui->cb->setChecked(false);
    ui->name->setText(ench.e.name);
    ui->sb->setMaximum(ench.mlvl);
    if(m > -1)
    {
        ui->sb->setMinimum(ench.e.lvl);
        ui->sb->setValue(ench.mlvl);
    }
    else
        ui->sb->setValue(ench.e.lvl);
}

void EnchListWidget::setEnchLevel(int lvl)  //设置魔咒等级
{
    ui->sb->setValue(lvl);
}

Ench EnchListWidget::getEnch()  //获取魔咒
{
    Ench e;
    e.name = ui->name->text();
    e.lvl = ui->sb->value();
    return e;
}

void EnchListWidget::setChecked(bool a) //设置选择状态
{
    ui->cb->setChecked(a);
}

bool EnchListWidget::isChecked()    //是否被选择
{
    return ui->cb->isChecked();
}

void EnchListWidget::checkRepulsion()   //检查冲突魔咒并应对
{
    struct EnchTable *t = BASE::sTable(ui->name->text());
    if(t == NULL)
        return;
    for(int i = 0; i < ench_list_l; i++)
    {
        for(int j = 0; j < t->repulsion->count(); j++)
        {
            if(ench_list[i].name == t->repulsion[j])
            {
                setEnabled(false);
                return;
            }
        }
    }
    if(!isEnabled())
    {
        setEnabled(true);
    }
}

