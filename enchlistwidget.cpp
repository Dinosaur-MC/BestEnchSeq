#include "enchlistwidget.h"
#include "ui_enchlistwidget.h"

EnchListWidget::EnchListWidget(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::EnchListWidget)
{
    ui->setupUi(this);

    connect(ui->cb, &QCheckBox::stateChanged, this, &EnchListWidget::stateChanged);
    connect(ui->sb, &QSpinBox::valueChanged, this, &EnchListWidget::valueChanged);
}

EnchListWidget::~EnchListWidget()
{
    delete ui;
}

void EnchListWidget::setEnch(EnchTable ench)
{
    ui->cb->setChecked(false);
    ui->name->setText(ench.name);
    ui->sb->setMaximum(ench.mlvl);
    ui->sb->setValue(1);
}

Ench EnchListWidget::ench()
{
    Ench e;
    e.name = ui->name->text();
    e.lvl = ui->sb->value();
    return e;
}

void EnchListWidget::setChecked(bool a)
{
    ui->cb->setChecked(a);
}

bool EnchListWidget::isChecked()
{
    return ui->cb->isChecked();
}

void EnchListWidget::checkRepulsion()
{
    int p = Basic::searchTable(ui->name->text());
    if(p == -1)
        return;
    for(int i = 0; i < Basic::origin_ench_l; i++)
    {
        for(int j = 0; j < Basic::ench_table[p].repulsion->count(); j++)
        {
            if(Basic::origin_ench[i].name == Basic::ench_table[p].repulsion[j])
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

