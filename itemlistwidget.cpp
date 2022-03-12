#include "itemlistwidget.h"
#include "ui_itemlistwidget.h"

ItemListWidget::ItemListWidget(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::ItemListWidget)
{
    ui->setupUi(this);

    connect(ui->cb, &QCheckBox::stateChanged, this, &ItemListWidget::stateChanged);
}

ItemListWidget::~ItemListWidget()
{
    delete ui;
}


void ItemListWidget::setItem(Item item)
{
    this_item = item;
    ui->cb->setChecked(true);
    if(item.name == ID_ECB)
    {
        ui->toolButton->setIcon(QIcon(":/icon/res/enchanted_book.png"));
        ui->weapon_name->setText("附魔书|Enchanted Book");
    }
    else if(item.name == "")
    {
        ui->cb->setHidden(true);
        ui->toolButton->setIcon(QIcon(":/icon/res/pack.png"));
        ui->weapon_name->setText("（双击添加物品 Double click to add item）");
        ui->ench_name->setHidden(true);
    }
    else
    {
        int p = Basic::searchWeapon(item.name);
        ui->toolButton->setIcon(Basic::weapon[p].icon);
        ui->weapon_name->setText(Basic::weapon[p].name.simplified());
    }
    ui->sb->setHidden(true);
}

Item ItemListWidget::item()
{
    return Item();
}

void ItemListWidget::setEnch(EnchPlus ench)
{
    ui->cb->setChecked(false);
    ui->toolButton->setIcon(QIcon(":/icon/res/enchanted_book.png"));
    ui->weapon_name->setHidden(true);
    ui->ench_name->setText(ench.e.name);
    ui->sb->setHidden(false);
    ui->sb->setMinimum(ench.e.lvl+1);
    ui->sb->setMaximum(ench.mlvl);
    ui->sb->setValue(ench.mlvl);
}

Ench ItemListWidget::ench()
{
    Ench e;
    e.name = ui->ench_name->text();
    e.lvl = ui->sb->value();
    return e;
}

void ItemListWidget::setChecked(bool a)
{
    ui->cb->setChecked(a);
}

bool ItemListWidget::isChecked()
{
    return ui->cb->isChecked();
}

void ItemListWidget::checkRepulsion()
{
    int p = Basic::searchTable(ui->ench_name->text());
    if(p == -1)
        return;
    for(int i = 0; i < Basic::needed_ench_l; i++)
    {
        for(int j = 0; j < Basic::ench_table[p].repulsion->count(); j++)
        {
            if(Basic::needed_ench[i].name == Basic::ench_table[p].repulsion[j])
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

void ItemListWidget::setLayoutMode(QLayout::SizeConstraint a)
{
    ui->horizontalLayout->setSizeConstraint(a);
}

