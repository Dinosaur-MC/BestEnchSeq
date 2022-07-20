#include "basicoperator.h"
#include "ui_itemwidget_ench.h"
#include "ui_itemwidget_item.h"
#include "ui_itemwidget_flowstep.h"

// ListWidget
ListWidget_Ench::ListWidget_Ench(QWidget *parent) :
    QListWidget{parent}
{

}


EnchPro ListWidget_Ench::getItem(int a)
{
    ItemWidget_Ench *w = (ItemWidget_Ench*)itemWidget(item(a));
    return w->item();
}

void ListWidget_Ench::pushItem(EnchPro epr)
{
    ItemWidget_Ench *w = new ItemWidget_Ench(this);
    w->setItem(epr);
    connect(w, &ItemWidget_Ench::stateChanged, this, &ListWidget_Ench::itemStateChanged);
    this->addItem("");
    this->setItemWidget(item(count()-1), w);
}


int ListWidget_Ench::checkedItemCount()
{
    int c = 0;
    for(int i = 0; i < count(); i++)
    {
        ItemWidget_Ench *w = (ItemWidget_Ench*)itemWidget(item(i));
        if(w->isChecked())
            c++;
    }
    return c;
}

void ListWidget_Ench::reload(QVector<EnchPro> eprs)
{
    this->clear();
    for(int i = 0; i < eprs.count(); i++)
        pushItem(eprs.at(i));
}




ListWidget_Item::ListWidget_Item(QWidget *parent) :
    QListWidget{parent}
{

}


Item ListWidget_Item::getItem(int a)
{
    ItemWidget_Item *w = (ItemWidget_Item*)itemWidget(item(a));
    return w->item();
}

void ListWidget_Item::pushItem(Item it)
{
    ItemWidget_Item *w = new ItemWidget_Item(this);
    w->setItem(it);
    connect(w, &ItemWidget_Item::stateChanged, this, &ListWidget_Item::itemStateChanged);
    this->addItem("");
    this->setItemWidget(item(count()-1), w);
}


int ListWidget_Item::checkedItemCount()
{
    int c = 0;
    for(int i = 0; i < count(); i++)
    {
        ItemWidget_Item *w = (ItemWidget_Item*)itemWidget(item(i));
        if(w->isChecked())
            c++;
    }
    return c;
}

void ListWidget_Item::reload(QVector<Item> items)
{
    this->clear();
    for(int i = 0; i < items.count(); i++)
        pushItem(items.at(i));
}




ListWidget_FlowStep::ListWidget_FlowStep(QWidget *parent) :
    QListWidget{parent}
{

}


FlowStep ListWidget_FlowStep::getItem(int a)
{
    ItemWidget_FlowStep *w = (ItemWidget_FlowStep*)itemWidget(item(a));
    return w->item();
}

void ListWidget_FlowStep::pushItem(FlowStep fs)
{
    ItemWidget_FlowStep *w = new ItemWidget_FlowStep(this);
    w->setItem(fs);
    connect(w, &ItemWidget_FlowStep::stateChanged, this, &ListWidget_FlowStep::itemStateChanged);
    this->addItem("");
    this->setItemWidget(item(count()-1), w);
}

void ListWidget_FlowStep::reload(QVector<FlowStep> fs)
{
    this->clear();
    for(int i = 0; i < fs.count(); i++)
        pushItem(fs.at(i));
}




// ItemWidget
ItemWidget_Ench::ItemWidget_Ench(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ItemWidget_Ench)
{
    ui->setupUi(this);
}

ItemWidget_Ench::~ItemWidget_Ench()
{
    delete ui;
}


EnchPro ItemWidget_Ench::item()
{
    return this_item;
}

void ItemWidget_Ench::setItem(EnchPro epr)
{
    ui->name->setText(epr.text);
    ui->sb->setValue(epr.maximum);  // Need more mode
    ui->sb->setMinimum(epr.minimum);
    ui->sb->setMaximum(epr.maximum);
}

void ItemWidget_Ench::setCheck(bool a)
{
    ui->cb->setChecked(a);
}

bool ItemWidget_Ench::isChecked()
{
    return ui->cb->isChecked();
}

void ItemWidget_Ench::setCheckboxHidden(bool a)
{
    ui->cb->setHidden(a);
}

void ItemWidget_Ench::setSpinboxHidden(bool a)
{
    ui->sb->setHidden(a);
}




ItemWidget_Item::ItemWidget_Item(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ItemWidget_Item)
{
    ui->setupUi(this);
}

ItemWidget_Item::~ItemWidget_Item()
{
    delete ui;
}


Item ItemWidget_Item::item()
{
    return this_item;
}

void ItemWidget_Item::setItem(Item it)
{
    if(it.type < 0)
    {
        ui->toolButton->setIcon(ICON_ECB);
        ui->weapon_name->setText(ID_ECB);
    }
    else
    {
        ui->toolButton->setIcon(raw_weapon_table.at(it.type).icon);
        ui->weapon_name->setText(raw_weapon_table.at(it.type).name);
    }

    QString data = "";
    if(it.ench.count() > 0)
    {
        data += raw_enchantment_table.at(it.ench.at(0).id).name + " " + numToRoman(it.ench.at(0).lvl);
        for(int i = 1; i < it.ench.count(); i++)
            data += "\n" + raw_enchantment_table.at(it.ench.at(i).id).name + " " + numToRoman(it.ench.at(i).lvl);
    }
    ui->ench_name->setText(data);
}

void ItemWidget_Item::setCheck(bool a)
{
    ui->cb->setChecked(a);
}

bool ItemWidget_Item::isChecked()
{
    return ui->cb->isChecked();
}

void ItemWidget_Item::setCheckboxHidden(bool a)
{
    ui->cb->setHidden(a);
}

void ItemWidget_Item::setSpinboxHidden(bool a)
{
    ui->sb->setHidden(a);
}




ItemWidget_FlowStep::ItemWidget_FlowStep(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ItemWidget_FlowStep)
{
    ui->setupUi(this);
}

ItemWidget_FlowStep::~ItemWidget_FlowStep()
{
    delete ui;
}


FlowStep ItemWidget_FlowStep::item()
{
    return this_item;
}

void ItemWidget_FlowStep::setItem(FlowStep fs)
{
    if(fs.a.type < 0)
        ui->iconA->setIcon(ICON_ECB);
    else
        ui->iconA->setIcon(raw_weapon_table.at(fs.a.type).icon);

    if(fs.b.type < 0)
        ui->iconB->setIcon(ICON_ECB);
    else
        ui->iconB->setIcon(raw_weapon_table.at(fs.b.type).icon);

    QString data = "";
    if(fs.a.ench.count() > 0)
    {
        data += raw_enchantment_table.at(fs.a.ench.at(0).id).name + " " + numToRoman(fs.a.ench.at(0).lvl);
        for(int i = 1; i < fs.a.ench.count(); i++)
            data += "\n" + raw_enchantment_table.at(fs.a.ench.at(i).id).name + " " + numToRoman(fs.a.ench.at(i).lvl);
    }
    ui->enchA->setText(data);

    data = "";
    if(fs.b.ench.count() > 0)
    {
        data += raw_enchantment_table.at(fs.b.ench.at(0).id).name + " " + numToRoman(fs.b.ench.at(0).lvl);
        for(int i = 1; i < fs.b.ench.count(); i++)
            data += "\n" + raw_enchantment_table.at(fs.b.ench.at(i).id).name + " " + numToRoman(fs.b.ench.at(i).lvl);
    }
    ui->enchB->setText(data);
}




WeaponBox::WeaponBox()
{

}

