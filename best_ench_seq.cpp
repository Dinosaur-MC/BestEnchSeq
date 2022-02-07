#include "best_ench_seq.h"
#include "ui_best_ench_seq.h"
#include <QFile>

using namespace std;

BestEnchSeq::BestEnchSeq(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BestEnchSeq)
{
    ui->setupUi(this);
    loadEnchantmentTable();

    //Input Item Type
    ui->InputItem->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->InputItem->setIconSize(QSize(64,64));
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_sword.png"), "剑\nSword");
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_pickaxe.png"), "镐\nPickaxe");
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_axe.png"), "斧\nAxe");
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_shovel.png"), "铲\nShovel");
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_hoe.png"), "锄\nHoe");
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_helmet.png"), "头盔\nHelmet");
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_chestplate.png"), "胸甲\nChestplate");
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_leggings.png"), "护腿\nLeggings");
    ui->InputItem->addItem(QIcon(":/icon/res/netherite_boots.png"), "靴\nBoots");
    ui->InputItem->addItem(QIcon(":/icon/res/bow.png"), "弓\nBow");
    ui->InputItem->addItem(QIcon(":/icon/res/crossbow_standby.png"), "弩\nCrossbow");
    ui->InputItem->addItem(QIcon(":/icon/res/trident.png"), "三叉戟\nTrident");
    ui->InputItem->addItem(QIcon(":/icon/res/fishing_rod.png"), "钓鱼竿\nFishing Rod");

    Common::InputItem.type = ui->InputItem->currentText();
    Common::InputItem.Durability = ui->Durability_0->value();
    Common::InputItem.Penalty = ui->Penalty_0->value();

    connect(ui->InputItem, &QComboBox::currentTextChanged, this, [=](){
        Common::InputItem.type = ui->InputItem->currentText();
        Common::InputItem.Durability = ui->Durability_0->value();
        Common::InputItem.Penalty = ui->Penalty_0->value();
        ui->OriginEnchantment->clearEnch();
        ui->NeededEnchantment->clearEnch();
    });
    connect(ui->Durability_0, &QSpinBox::valueChanged, this, [=](){
        Common::InputItem.Durability = ui->Durability_0->value();
    });
    connect(ui->Penalty_0, &QSpinBox::valueChanged, this, [=](){
        Common::InputItem.Penalty = ui->Penalty_0->value();
    });

    connect(ui->OriginEnchantment, &QListWidget::itemChanged, ui->NeededEnchantment, [=](){
        ui->NeededEnchantment->clearEnch();
    });
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](){
        ui->OriginEnchantment->refreshEnch();
    });
    ui->NeededEnchantment->setMode(1);

    //Output Item
    ui->OutputItem->setIconSize(QSize(64,64));
    ui->OutputItem->setIcon(QIcon(":/icon/res/pack.png"));

    //Processing Mode
    connect(ui->radioEB, &QRadioButton::clicked, this, [=](){
        Common::processing_mode = 0;
    });
    connect(ui->radioEB_IC, &QRadioButton::clicked, this, [=](){
        Common::processing_mode = 1;
    });
    connect(ui->radioAC, &QRadioButton::clicked, this, [=](){
        Common::processing_mode = 2;
    });

    //Buttons
    connect(ui->btnReset, &QPushButton::clicked, this, [=](){
        qApp->exit(1314);
    });
    connect(ui->btnNext_1, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(1);
    });

    connect(ui->btnBack_2, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(0);
    });

    connect(ui->btnCalc_2, &QPushButton::clicked, this, [=](){
        if(ui->NeededEnchantment->count() > 1)
        {
            ui->tabWidget->setCurrentIndex(2);
            if(Common::processing_mode == 0)
                calculate();
        }
        else
        {

        }
    });

    connect(ui->btnBack_3, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(1);
    });

    connect(ui->btnSave, &QPushButton::clicked, this, [=](){

    });
}

BestEnchSeq::~BestEnchSeq()
{
    delete ui;
}

void BestEnchSeq::loadEnchantmentTable()
{
    qDebug() << "Loading enchantments infomaton...";
    QFile file("EnchantmentTable.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error: cannot find EnchantmentTable.txt";
        return;
    }
    QStringList str = QString(file.readAll()).split('\n');
    file.close();

    for(int i = 0; i < str.count(); i++)
    {
        QStringList str2 = str[i].split(' ');
        Common::Table[i].name = str2[0];
        Common::Table[i].max_lvl = str2[1].toInt();
        Common::Table[i].multiplier[0] = str2[2].toInt();
        Common::Table[i].multiplier[1] = str2[3].toInt();
        for(int j = 0; j < 13; j++)
        {
            Common::Table[i].suitable[j] = str2[j+4].toInt();
        }
        Common::Table[i].repulsion = str2[17].split(",");
    }
    qDebug() << "Enchantments infomaton has been loaded!";

/*
    for(int i = 0; &Common::Table[i] != end(Common::Table) && Common::Table[i].name != ""; i++)
    {
        qDebug() << i << Common::Table[i].name << Common::Table[i].max_lvl << Common::Table[i].multiplier[0] << Common::Table[i].multiplier[1] << Common::Table[i].suitable;
        for(int j = 0; j < 13; j++)
        {
            qDebug() << i << " " << j << Common::Table[i].suitable[j];
        }
    }
*/
}

void BestEnchSeq::calculate()
{
    qDebug() << "Calculating..";

    Ench *a, *b;
    a = ui->OriginEnchantment->getEnch();
    b = ui->NeededEnchantment->getEnch();

    qDebug() << "Step 1";
    for(int i = 0; i < 64 && a[i].name != ""; i++)
    {
        Common::InputItem.ench[i] = a[i];
        Common::OutputItem.ench[i] = a[i];
    }
    if(Common::OutputItem.ench[0].name != "")
    {
        for(int i = 0; i < 64 && b[i].name != ""; i++)
        {
            int p = Common::searchEach(Common::OutputItem.ench, 64, b[i].name);
            if(p == -1)
            {
                int j = 0;
                while(j < 64 && Common::OutputItem.ench[j].name != "")
                    j++;
                Common::OutputItem.ench[j] = b[i];
            }
            else
            {
                Common::OutputItem.ench[p].lvl = b[i].lvl;
            }
        }
    }

    qDebug() << "Step 2";
    Item Need[64];
    for(int i = 0, j = 0; i < 64 && j < 64 && Common::OutputItem.ench[j].name != ""; j++)
    {
        int p = Common::searchEach(Common::InputItem.ench, 64, Common::OutputItem.ench[j].name);
        if(p== -1)
        {
            Need[i].ench[0] = Common::OutputItem.ench[j];
            i++;
        }
        else if(Common::OutputItem.ench[j].lvl != Common::InputItem.ench[p].lvl)
        {
            if(Common::OutputItem.ench[j].lvl - Common::InputItem.ench[p].lvl == 1)
            {
                Need[i].ench[0] = Common::InputItem.ench[j];
            }
            else
            {
                Need[i].ench[0] = Common::OutputItem.ench[j];
            }
            i++;
        }
    }

    qDebug() << "Step 3";
    Step list[64];
    listMinSet(list, 64, Common::InputItem, Need, 64, minCost(Common::InputItem, Need, 64));

}

int BestEnchSeq::minCost(Item target, Item *items, int len)
{
    int min_cost = 2147483647;

    for(int i = 0; i < len; i++)
    {
        int cost = preForgeItem(target, items[i]);
        if(cost < min_cost)
        {
            min_cost = cost;
        }
    }

    for(int i = 0; i < len; i++)
    {
        for(int j = 0; j < len; j++)
        {
            if(i != j)
            {
                int cost = preForgeItem(items[i], items[j]);
                if(cost < min_cost)
                {
                    min_cost = cost;
                }
            }
        }
    }

    return min_cost;
}

void BestEnchSeq::listMinSet(Step *list, int cap, Item target, Item *items, int len, int min_cost)
{
    int p = 0;

    for(int i = 0; i < len && p < cap; i++)
    {
        int cost = preForgeItem(target, items[i]);
        if(cost == min_cost)
        {
            list[p].tar = target;
            list[p].sac = items[i];
            list[p].penalty = max(target.Penalty, items[i].Penalty) + 1;
            list[p].cost = cost;
            p++;
        }
    }

    for(int i = 0; i < len && p < cap; i++)
    {
        for(int j = 0; i < len && p < cap; j++)
        {
            if(i != j)
            {
                int cost = preForgeItem(items[i], items[j]);
                if(cost == min_cost)
                {
                    list[p].tar = items[i];
                    list[p].sac = items[j];
                    list[p].penalty = max(target.Penalty, items[i].Penalty) + 1;
                    list[p].cost = cost;
                    p++;
                }
            }
        }
    }
}

int BestEnchSeq::preForgeItem(Item target, Item sacrifice)
{
    int cost = 0;

    for(int i = 0; &sacrifice.ench[i] != end(sacrifice.ench) && sacrifice.ench[i].name != ""; i++)
    {
        int p = Common::searchEach(target.ench, 64, sacrifice.ench[i].name);
        qDebug() << "preForgeItem:" << i << sacrifice.ench[i].name << p;
        if(p == -1)
        {
            cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[0] * sacrifice.ench[i].lvl;
        }
        else
        {
            if(target.ench[p].lvl == sacrifice.ench[i].lvl)
            {
                cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[0] * (sacrifice.ench[i].lvl + 1);
            }
            else if(target.ench[p].lvl < sacrifice.ench[i].lvl)
            {
                cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[0] * sacrifice.ench[i].lvl;
            }
        }
    }
    cost += pow(2, target.Penalty) - 1;
    cost += pow(2, sacrifice.Penalty) - 1;

    return cost;
}

Item BestEnchSeq::forgeItem(Item target, Item sacrifice)
{
    for(int i = 0; &sacrifice.ench[i] != end(sacrifice.ench) && sacrifice.ench[i].name != ""; i++)
    {
        int p = Common::searchEach(target.ench, 64, sacrifice.ench[i].name);
        if(p == -1)
        {
            int p = 0;
            while(target.ench[p].name != "" && &target.ench[p] != end(target.ench))
                p++;
            target.ench[p] = sacrifice.ench[i];
        }
        else
        {
            if(target.ench[p].lvl == sacrifice.ench[i].lvl)
            {
                target.ench[p].lvl += 1;
            }
            else if(target.ench[p].lvl < sacrifice.ench[i].lvl)
            {
                target.ench[p].lvl = sacrifice.ench[i].lvl;
            }
        }
    }

    target.Penalty = max(target.Penalty, sacrifice.Penalty) + 1;
    return target;
}

