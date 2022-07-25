#include "basiclists.h"
#include "basicoperator.h"
#include "ui_itemwidget_ench.h"
#include "ui_itemwidget_item.h"
#include "ui_itemwidget_flowstep.h"


// ListWidget
ListWidget_Ench::ListWidget_Ench(QWidget *parent) :
    QListWidget{parent}
{

}


ItemWidget_Ench* ListWidget_Ench::getItem(int a)
{
    return (ItemWidget_Ench*)itemWidget(item(a));
}

void ListWidget_Ench::pushItem(EnchPro epr)
{
    ItemWidget_Ench *w = new ItemWidget_Ench(this);
    w->setItem(epr);
    QListWidgetItem *it = new QListWidgetItem();
    it->setSizeHint(w->sizeHint());
    this->addItem(it);
    this->setItemWidget(it, w);
    connect(w, &ItemWidget_Ench::stateChanged, this, &ListWidget_Ench::itemStateChanged);
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

QVector<Ench> ListWidget_Ench::getCheckedItem()
{
    QVector<Ench> es;
    for(int i = 0; i < count(); i++)
    {
        ItemWidget_Ench *w = (ItemWidget_Ench*)itemWidget(item(i));
        if(w->isChecked())
            es.append(w->item());
    }
    return es;
}

void ListWidget_Ench::reload(const QVector<EnchPro> *eprs)
{
    this->clear();
    for(int i = 0; i < eprs->count(); i++)
        pushItem(eprs->at(i));
}




ListWidget_Item::ListWidget_Item(QWidget *parent) :
    QListWidget{parent}
{

}


ItemWidget_Item* ListWidget_Item::getItem(int a)
{
    return (ItemWidget_Item*)itemWidget(item(a));
}

void ListWidget_Item::pushItem(Item it, ItemPro itpr)
{
    ItemWidget_Item *w = new ItemWidget_Item(this);
    w->setItem(it, itpr);
    QListWidgetItem *ite = new QListWidgetItem();
    ite->setSizeHint(w->sizeHint());
    this->addItem(ite);
    this->setItemWidget(ite, w);
    connect(w, &ItemWidget_Item::stateChanged, this, &ListWidget_Item::itemStateChanged);
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

QVector<Item> ListWidget_Item::getCheckedItem()
{
    QVector<Item> its;
    for(int i = 0; i < count(); i++)
    {
        ItemWidget_Item *w = (ItemWidget_Item*)itemWidget(item(i));
        if(w->isChecked())
            its.append(w->item());
    }
    return its;
}

void ListWidget_Item::reload(const QVector<Item> *its, const QVector<ItemPro> *itprs)
{
    this->clear();
    for(int i = 0; i < itprs->count(); i++)
        pushItem(its->at(i), itprs->at(i));
}




ListWidget_FlowStep::ListWidget_FlowStep(QWidget *parent) :
    QListWidget{parent}
{

}


ItemWidget_FlowStep* ListWidget_FlowStep::getItem(int a)
{
    return (ItemWidget_FlowStep*)itemWidget(item(a));
}

void ListWidget_FlowStep::pushItem(FlowStep fs, FlowStepPro fspr)
{
    ItemWidget_FlowStep *w = new ItemWidget_FlowStep(this);
    w->setItem(fs, fspr);
    QListWidgetItem *it = new QListWidgetItem();
    it->setSizeHint(w->sizeHint());
    this->addItem(it);
    this->setItemWidget(it, w);
}

void ListWidget_FlowStep::reload(const QVector<FlowStep> *fses, const QVector<FlowStepPro> *fsprs)
{
    this->clear();
    for(int i = 0; i < fsprs->count(); i++)
        pushItem(fses->at(i), fsprs->at(i));
}




// ItemWidget
ItemWidget_Ench::ItemWidget_Ench(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ItemWidget_Ench)
{
    ui->setupUi(this);
    connect(ui->cb, &QCheckBox::stateChanged, this, &ItemWidget_Ench::stateChanged);
}

ItemWidget_Ench::~ItemWidget_Ench()
{
    delete ui;
}


void ItemWidget_Ench::setMode(int m)
{
    mode = m;
}

Ench ItemWidget_Ench::item()
{
    Ench e;
    e.id = this_item.id;
    e.lvl = ui->sb->value();
    return e;
}

void ItemWidget_Ench::setItem(EnchPro epr)
{
    this_item = epr;
    ui->name->setText(epr.text);
    if(mode == 0)
        ui->sb->setValue(epr.minimum);
    else
        ui->sb->setValue(epr.maximum);
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
    connect(ui->cb, &QCheckBox::stateChanged, this, &ItemWidget_Item::stateChanged);
}

ItemWidget_Item::~ItemWidget_Item()
{
    delete ui;
}


Item ItemWidget_Item::item()
{
    return this_item;
}

void ItemWidget_Item::setItem(Item it, ItemPro itpr)
{
    this_item = it;
    ui->icon->setIcon(itpr.icon);
    ui->weapon_name->setText(itpr.name);
    ui->ench_set->setText(itpr.ench_set);
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

void ItemWidget_FlowStep::setItem(FlowStep fs, FlowStepPro fsp)
{
    this_item = fs;
    ui->iconA->setIcon(fsp.a.icon);
    ui->enchA->setText(fsp.a.ench_set);
    ui->iconB->setIcon(fsp.b.icon);
    ui->enchB->setText(fsp.b.ench_set);
    ui->cost->setText(QString::number(fsp.levelCost));
}




WeaponBox::WeaponBox(QWidget *parent)
    : QComboBox{parent}
{
    qDebug() << "[WeaponBox] Initialized.";
}


void WeaponBox::addWeapon(raw_Weapon rw)
{
    this->addItem(rw.icon, rw.name);
}

void WeaponBox::reload(const QVector<raw_Weapon> *rws)
{
    this->clear();
    for(int i = 0; i < rws->count(); i++)
        addWeapon(rws->at(i));
}

raw_Weapon WeaponBox::currentWeapon()
{
    int p = this->currentIndex();

    raw_Weapon rw;
    rw.icon = this->itemIcon(p);
    rw.name = this->itemText(p);

    return rw;
}

