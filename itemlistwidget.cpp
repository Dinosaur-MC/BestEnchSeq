#include "itemlistwidget.h"
#include "ui_itemlistwidget.h"

ItemListWidget::ItemListWidget(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::ItemListWidget)
{
    ui->setupUi(this);

    connect(ui->cb, &QCheckBox::stateChanged, this, &ItemListWidget::stateChanged);
    this_item = {};
}

ItemListWidget::~ItemListWidget()
{
    delete ui;
}


void ItemListWidget::setItem(Item item)
{
    this_item = item;

    ui->cb->setChecked(true);
    ui->sb->setHidden(true);
    ui->ench_name->setHidden(false);
    if(item.name == ID_ECB)
    {
        ui->toolButton->setIcon(QIcon(":/icon/res/enchanted_book.png"));
        ui->weapon_name->setText("附魔书 Enchanted Book");
    }
    else
    {
        struct Weapon *p = BASE::sWeapon(item.name);
        if(p == nullptr)
        {
            ui->cb->setHidden(true);
            ui->toolButton->setIcon(QIcon(":/icon/res/pack.png"));
            ui->weapon_name->setText("（双击添加物品 Double click to add item）");
            ui->ench_name->setHidden(true);
            return;
        }
        else
        {
            ui->toolButton->setIcon(p->icon);
            ui->weapon_name->setText(p->name.simplified());
        }
    }

    if(item.ench[0].name.isEmpty())
    {
        ui->ench_name->setHidden(true);
        ui->ench_name->setText("");
    }
    else
    {
        ui->ench_name->setHidden(false);
        QString str;
        for(int i = 0; i < INIT_LENGTH && !item.ench[i].name.isEmpty(); i++)
        {
            str += item.ench[i].name + " ";
            str += BASE::IntToRoman(item.ench[i].lvl);
            if(i != INIT_LENGTH-1 && !item.ench[i+1].name.isEmpty())
                str += "\n";
        }
        ui->ench_name->setText(str);
    }
}

Item ItemListWidget::item()
{
    return this_item;
}

void ItemListWidget::setChecked(bool a)
{
    ui->cb->setChecked(a);
}

bool ItemListWidget::isChecked()
{
    return ui->cb->isChecked();
}

