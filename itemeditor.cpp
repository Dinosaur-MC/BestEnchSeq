#include "itemeditor.h"
#include "ui_itemeditor.h"
#include "enchlist.h"

int ItemEditor::enchlist_l = 0;
Ench *ItemEditor::enchlist = nullptr;

ItemEditor::ItemEditor(QWidget *parent)
    : QDialog{parent}
    , ui(new Ui::ItemEditor)
{
    qDebug() << "[ItemEditor]";
    ui->setupUi(this);
    setWindowTitle("Item Editor");

    enchlist_l = 0;
    enchlist = nullptr;

    ui->sbDuration->setEnabled(false);
    ui->enchlist->setOutputAddress(enchlist, resizeEnchList);
    ui->enchlist->setWeapon();

    connect(ui->radioEB, &QRadioButton::clicked, this, [=](){
        ui->sbDuration->setEnabled(false);
        ui->sbDuration->setValue(0);
        ui->enchlist->setWeapon();
    });
    connect(ui->radioW, &QRadioButton::clicked, this, [=](){
        ui->sbDuration->setEnabled(true);
        ui->sbDuration->setValue(100);
        ui->enchlist->setWeapon(DM->OriginItem->name);
    });

    connect(ui->btnConfirm, &QPushButton::clicked, this, [=](){
        if(ui->radioEB->isChecked())
        {
            if(enchlist_l == 0)
            {
                return;
            }
            this_item.name = ID_ECB;
            this_item.durability = 0;
        }
        else
        {
            this_item.name = DM->OriginItem->name;
            this_item.durability = ui->sbDuration->value();
        }
        this_item.penalty = ui->sbPenalty->value();
        qDebug() << "ui->btnConfirm" << enchlist_l;
        for(int i = 0; i < INIT_LENGTH && !this_item.ench[i].name.isEmpty(); i++)
            this_item.ench[i] = {"",0};
        for(int i = 0; i < INIT_LENGTH && i < enchlist_l; i++)
        {
            this_item.ench[i] = enchlist[i];
        }

        accept();
    });
    connect(ui->btnCnacel, &QPushButton::clicked, this, &QDialog::reject);
}

ItemEditor::~ItemEditor()
{
    delete ui;
}


void ItemEditor::setItem(Item item)
{
    this_item = item;

    if(item.name == ID_ECB || item.name.isEmpty())
    {
        ui->enchlist->refresh();
        ui->radioEB->setChecked(true);
    }
    else
    {
        ui->enchlist->setWeapon(DM->OriginItem->name);
        ui->radioW->setChecked(true);
        ui->sbDuration->setEnabled(true);
    }
    ui->sbDuration->setValue(item.durability);
    ui->sbPenalty->setValue(item.penalty);

    for(int i = 0; i < INIT_LENGTH && !item.ench[i].name.isEmpty() ; i++)
    {
        for(int j = 0; j < ui->enchlist->count(); j++)
        {
            EnchListWidget *w = (EnchListWidget*)ui->enchlist->itemWidget(ui->enchlist->item(j));
            if(item.ench[i].name == w->getEnch().name)
            {
                w->setEnchLevel(item.ench[i].lvl);
                w->setChecked(true);
                break;
            }
        }
    }
}

Item ItemEditor::item()
{
    return this_item;
}

Ench *ItemEditor::resizeEnchList(int len)
{
    enchlist_l = len;
    delete [] enchlist;
    enchlist = new Ench[len];
    return enchlist;
}

