#include "best_ench_seq.h"
#include "ui_best_ench_seq.h"
#include <QFile>
#include <QDesktopServices>
#include <QDateTime>

using namespace std;

BestEnchSeq::BestEnchSeq(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BestEnchSeq)
{
    ui->setupUi(this);
    loadEnchantmentTable();

    //Menubar
    connect(ui->actionInport, &QAction::triggered, this, [=](){

    });
    connect(ui->actionExport, &QAction::triggered, this, [=](){

    });
    connect(ui->actionExit, &QAction::triggered, this, &BestEnchSeq::close);
    connect(ui->actionRefresh, &QAction::triggered, this, [=](){
        loadEnchantmentTable();
    });
    connect(ui->actionVersion, &QAction::triggered, this, [=](){
        QWidget *v = new QWidget();
        v->setWindowTitle("Version");
        v->setFixedSize(256,80);
        QLabel *ver = new QLabel(v);
        ver->setText("Version: 1.0beta\n Author: Dinosaur_MC\n(https://space.bilibili.com/85607675)");
        ver->move(20,12);
        v->show();
    });
    connect(ui->actionWeb, &QAction::triggered, this, [=](){
        QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/Dinosaur-MC/BestEnchSeq")));
    });

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

    connect(ui->cbIgnoreFixing, &QCheckBox::stateChanged, this, [=](){
        if(ui->cbIgnoreFixing->isChecked())
        {
            Common::InputItem.Durability = 0;
            Common::ignore_Fixing = 1;
        }
        else
        {
            Common::InputItem.Durability = ui->Durability_0->value();
            Common::ignore_Fixing = 0;
        }
    });
    connect(ui->cbIgnorePWP, &QCheckBox::stateChanged, this, [=](){
        if(ui->cbIgnorePWP->isChecked())
        {
            Common::InputItem.Penalty = 0;
            Common::ignore_PWP = 1;
        }
        else
        {
            Common::InputItem.Penalty = ui->Penalty_0->value();
            Common::ignore_PWP = 0;
        }
    });
    connect(ui->cbIgnoreCL, &QCheckBox::stateChanged, this, [=](){
        if(ui->cbIgnoreCL->isChecked())
            Common::ignore_CL = 1;
        else
            Common::ignore_CL = 0;
    });

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
            {
                for(int i = 0; &Common::Flow[i] != end(Common::Flow) ; i++)
                    Common::Flow[i] = {};
                ui->EnchantFlow->clear();
                ui->EnchantFlow->refresh();
                calculate();
            }
        }
        else
        {
            ui->NeededEnchantment->item(0)->setForeground(QBrush(QColor(255,0,0)));
        }
        ui->OutputItem->setIcon(ui->InputItem->itemIcon(ui->InputItem->currentIndex()));
    });

    connect(ui->btnBack_3, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(1);
    });

    connect(ui->btnSave, &QPushButton::clicked, this, [=](){
        if(ui->EnchantFlow->count() != 0)
        {
            QString name = QString("output_") + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmsszzz") + ".txt";
            QFile file(name);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "Error: cannot output the file!";
                return;
            }
            file.write(QString("--总花费等级AllCost: [" + ui->CostLevel->text() + "]--\n").toUtf8().data());
            for(int i = 0; i < ui->EnchantFlow->count(); i++)
            {
                QString temp = "\n(" + QString::number(i) + ")  花费Cost: " + QString::number(Common::Flow[i].cost) + "\n";

                if(Common::Flow[i].tar.Durability == 0)
                    temp += "A. 附魔书 EnchantedBook\n";
                else
                    temp += "A. 目标物品 TargetItem\n";
                for(int j = 0; &Common::Flow[i].tar.ench[j] != end(Common::Flow[i].tar.ench) && Common::Flow[i].tar.ench[j].name != ""; j++)
                {
                    temp += "---" + Common::Flow[i].tar.ench[j].name + " [" + QString::number(Common::Flow[i].tar.ench[j].lvl) + "]\n";
                }

                temp += "B. 附魔书 EnchantedBook\n";
                for(int j = 0; &Common::Flow[i].sac.ench[j] != end(Common::Flow[i].sac.ench) && Common::Flow[i].sac.ench[j].name != ""; j++)
                {
                    temp += "---" + Common::Flow[i].sac.ench[j].name + " [" + QString::number(Common::Flow[i].sac.ench[j].lvl) + "]\n";
                }

                file.write(temp.toUtf8().data());
            }
            file.close();
            QDesktopServices::openUrl(QUrl::fromLocalFile(name));
        }
    });
}

BestEnchSeq::~BestEnchSeq()
{
    delete ui;
    Common::ignore_Fixing = 0;
    Common::ignore_PWP = 0;
    Common::ignore_CL = 0;
    Common::processing_mode = 0;
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
    for(int i = 0; i < 64; i++)
    {
        Common::InputItem.ench[i] = a[i];
        Common::OutputItem.ench[i] = a[i];
    }
    Common::InputItem.Durability = ui->Durability_0->value();
    Common::InputItem.Penalty = ui->Penalty_0->value();
    if(b[0].name != "")
    {
        for(int i = 0; i < 64 && b[i].name != ""; i++)
        {
            int p = Common::searchEach(Common::InputItem.ench, 64, b[i].name);
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
    for(int i = 0; Common::InputItem.ench[i].name != ""; i++)
    {
        qDebug() << i << "InputItem.ench" << Common::InputItem.ench[i].name << a[i].name;
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

    for(int i = 0; &Need[i] != end(Need) && Need[i].ench[0].name != ""; i++)
    {
        qDebug() << i << "Need:";
        for(int j = 0; &Need[i].ench[j] != end(Need[i].ench) && Need[i].ench[j].name != ""; j++)
        {
            qDebug() << Need[i].ench[j].name << Need[i].ench[j].lvl;
        }
    }

    qDebug() << "Step 3";
    ui->Notice->setText("");
    Item empty, Output;
    int allCost = 0;
    int I = 0;
    while(Need[0].ench[0].name != "" && I < 64)
    {
        Step list[64];

        listMinSet(list, 64, Common::InputItem, Need, 64, minCost(Common::InputItem, Need, 64));

        if(!Common::ignore_PWP)
            listMinPenalty(list, 64, minPenalty(list, 64));

        Step currentStep = minSumLevel(list, 64);
        Common::Flow[I] = currentStep;

        Item newItem = forgeItem(currentStep.tar, currentStep.sac);
        for(int i = 0; &Need[i] != end(Need) && Need[i].ench[0].name != "" && currentStep.tar.Durability == 0; i++)
        {
            if(Common::compareEnch(Need[i].ench, currentStep.tar.ench, 64) == 0)
            {
                int j = 0;
                for(; &Need[i+j+1] != end(Need) && Need[i+j].ench[0].name != ""; j++)
                {
                    Need[i+j] = Need[i+j+1];
                }
                Need[i+j] = empty;
            }
        }
        for(int i = 0; &Need[i] != end(Need) && Need[i].ench[0].name != "" && currentStep.sac.Durability == 0; i++)
        {
            if(Common::compareEnch(Need[i].ench, currentStep.sac.ench, 64) == 0)
            {
                int j = 0;
                for(; &Need[i+j+1] != end(Need) && Need[i+j].ench[0].name != ""; j++)
                {
                    Need[i+j] = Need[i+j+1];
                }
                Need[i+j] = empty;

            }
        }
        if(newItem.Durability == 0)
        {
            int i = 0;
            while(&Need[i] != end(Need) && Need[i].ench[0].name != "")
            {
                i++;
            }
            if(&Need[i] != end(Need))
                Need[i] = newItem;
        }
        else
        {
            Common::InputItem = newItem;
        }

        allCost += currentStep.cost;
        Output = newItem;

        if(!Common::ignore_CL)
        {
            if(currentStep.cost < 40 && ui->Notice->text() != "过于昂贵！\nToo Expensive!")
                ui->Notice->setText("");
            else
                ui->Notice->setText("过于昂贵！\nToo Expensive!");
        }

        //Flow Chart
        ui->EnchantFlow->addStep(currentStep, ui->InputItem->itemIcon(ui->InputItem->currentIndex()));
        ui->EnchantFlow->refreshSize();

        //Debug Output
        for(int i = 0; &Need[i] != end(Need) && Need[i].ench[0].name != ""; i++)
        {
            QString temp = "";
            for(int j = 0; &Need[i].ench[j] != end(Need[i].ench) && Need[i].ench[j].name != ""; j++)
            {
                temp += Need[i].ench[j].name + "<";
                temp += QString::number(Need[i].ench[j].lvl) + "> ";
            }
            qDebug() << I << i << "Need:" << temp;
        }
        I++;
    }

    if(Common::ignore_Fixing)
        ui->Durability_1->setText("Ig");
    else
        ui->Durability_1->setText(QString::number(Output.Durability));
    if(Common::ignore_PWP)
        ui->Penalty_1->setText("Ig");
    else
        ui->Penalty_1->setText(QString::number(Output.Penalty));
    ui->CostLevel->setText(QString::number(allCost));
    ui->StepCount->setText(QString::number(I));

    qDebug() << "Calculate End";
}

int BestEnchSeq::minCost(Item target, Item *items, int len)
{
    int min_cost = 2147483647;

    for(int i = 0; i < len && items[i].ench[0].name != ""; i++)
    {
        int cost = preForgeItem(target, items[i]);
        if(cost < min_cost)
        {
            min_cost = cost;
        }
    }

    for(int i = 0; i < len && items[i].ench[0].name != ""; i++)
    {
        for(int j = 0; j < len && items[j].ench[0].name != ""; j++)
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

    for(int i = 0; i < len && items[i].ench[0].name != "" && p < cap; i++)
    {
        int cost = preForgeItem(target, items[i]);
        if(cost == min_cost)
        {
            list[p].tar = target;
            list[p].sac = items[i];
            if(!Common::ignore_PWP)
                list[p].penalty = max(target.Penalty, items[i].Penalty) + 1;
            list[p].cost = cost;
            p++;
        }
    }

    for(int i = 0; i < len && items[i].ench[0].name != "" && p < cap; i++)
    {
        for(int j = 0; j < len && items[j].ench[0].name != "" && p < cap; j++)
        {
            if(i != j)
            {
                int cost = preForgeItem(items[i], items[j]);
                if(cost == min_cost)
                {
                    list[p].tar = items[i];
                    list[p].sac = items[j];
                    if(!Common::ignore_PWP)
                        list[p].penalty = max(items[i].Penalty, items[j].Penalty) + 1;
                    list[p].cost = cost;
                    p++;
                }
            }
        }
    }
}

int BestEnchSeq::minPenalty(Step *list, int len)
{
    if(Common::ignore_PWP)
        return 0;
    int min_penalty = 32;
    for(int i = 0; i < len && list[i].sac.ench[0].name != ""; i++)
    {
        if(list[i].penalty < min_penalty)
        {
            min_penalty = list[i].penalty;
        }
    }
    return min_penalty;
}

void BestEnchSeq::listMinPenalty(Step *list, int len, int min_penalty)
{
    for(int i = 0; i < len && list[i].sac.ench[0].name != ""; i++)
    {
        if(list[i].penalty != min_penalty)
        {
            int j = 0;
            for(; i + j + 1 < len && list[i+j].sac.ench[0].name != ""; j++)
            {
                list[i+j] = list[i+j+1];
            }
            list[i+j] = {};
        }
    }
}

Step BestEnchSeq::minSumLevel(Step *list, int len)
{
    int min_sum_level = 2147483647;
    for(int i = 0; i < len && list[i].sac.ench[0].name != ""; i++)
    {
        int sum_level = preForgeItem(Item(), list[i].tar) + preForgeItem(Item({}), list[i].sac);
        if(sum_level < min_sum_level)
        {
            min_sum_level = sum_level;
        }
    }

    for(int i = 0; i < len && list[i].sac.ench[0].name != ""; i++)
    {
        int sum_level = preForgeItem(Item(), list[i].tar) + preForgeItem(Item({}), list[i].sac);
        if(sum_level == min_sum_level)
        {
            return list[i];
        }
    }
    return Step();
}

int BestEnchSeq::preForgeItem(Item target, Item sacrifice)
{
    int cost = 0;

    for(int i = 0; &sacrifice.ench[i] != end(sacrifice.ench) && sacrifice.ench[i].name != ""; i++)
    {
        int p = Common::searchEach(target.ench, 64, sacrifice.ench[i].name);
//        qDebug() << "preForgeItem:" << i << sacrifice.ench[i].name << p;
        if(p == -1)
        {
            if(sacrifice.Durability != 0)
                cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[0] * sacrifice.ench[i].lvl;
            else
                cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[1] * sacrifice.ench[i].lvl;
        }
        else
        {
            if(target.ench[p].lvl == sacrifice.ench[i].lvl)
            {
                if(sacrifice.Durability != 0)
                    cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[0] * (sacrifice.ench[i].lvl + 1);
                else
                    cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[1] * (sacrifice.ench[i].lvl + 1);
            }
            else if(target.ench[p].lvl < sacrifice.ench[i].lvl)
            {
                if(sacrifice.Durability != 0)
                    cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[0] * sacrifice.ench[i].lvl;
                else
                    cost += Common::Table[Common::searchTable(sacrifice.ench[i].name)].multiplier[1] * sacrifice.ench[i].lvl;
            }
        }
    }
    if(!Common::ignore_PWP)
    {
        cost += pow(2, target.Penalty) - 1;
        cost += pow(2, sacrifice.Penalty) - 1;
    }

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

    if(!Common::ignore_PWP)
        target.Penalty = max(target.Penalty, sacrifice.Penalty) + 1;
    return target;
}

