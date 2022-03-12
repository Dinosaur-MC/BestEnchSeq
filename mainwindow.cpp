#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basic.h"
#include "settings.h"
#include "fileoperate.h"
#include "enchlist.h"
#include "itemlist.h"
#include "flowlist.h"
#include "calculator.h"
#include "checkupdate.h"

#include <QUrl>
#include <QDesktopServices>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString(PROGRAM_NAME_CN) + "-" + PROGRAM_NAME_EN + " v" + VERSION + " [" + AUTHOR + "]");
    QLabel *labText = new QLabel(this);
    labText->setText(STATEMENT);
    ui->statusBar->addWidget(labText);


    //Menu Bar Connection
    connect(ui->actionSettings, &QAction::triggered, this, [=](){
        Settings *w = new Settings(this);
        w->setWindowTitle("设置 Settings");
        w->show();
        if(w->exec() == Settings::Accepted)
            refresh();
    });
    connect(ui->actionHelp, &QAction::triggered, this, [=]{
        QDesktopServices::openUrl(QUrl(QLatin1String("https://www.bilibili.com/video/BV11T4y1D7c2")));
    });
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(ui->actionWeapon, &QAction::triggered, this, [=](){

    });
    connect(ui->actionEnchantment, &QAction::triggered, this, [=](){

    });
    connect(ui->actionVersion, &QAction::triggered, this, [=](){
        QDialog w;
        QLabel *name = new QLabel(QString("* * * ") + PROGRAM_NAME_CN + " * * *\n* * * " + PROGRAM_NAME_EN + " * * *\n", &w);
        name->setAlignment(Qt::AlignHCenter);
        QLabel *ver = new QLabel(QString("Version: ") + VERSION, &w);
        ver->setAlignment(Qt::AlignHCenter);
        QLabel *author = new QLabel(QString("Author: ") + AUTHOR, &w);
        author->setAlignment(Qt::AlignHCenter);
        QPushButton *btn = new QPushButton("确定 Confirm", &w);
        connect(btn, &QPushButton::clicked, &w, &QDialog::accept);

        QVBoxLayout *layout = new QVBoxLayout(&w);
        layout->addWidget(name);
        layout->addWidget(ver);
        layout->addWidget(author);
        layout->addWidget(btn);

        w.setFixedSize(320, 140);
        w.setLayout(layout);
        w.setModal(true);
        w.show();
        w.exec();
    });
    connect(ui->actionWebsite, &QAction::triggered, this, [=](){
        QDesktopServices::openUrl(QUrl(QLatin1String(WEBSITE)));
    });

    //Tab Connection
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](){
        if(ui->OriginEnchantment->isChanged())
            ui->NeededEnchantment->setWeapon(Basic::weapon[ui->InputItem->currentIndex()].name);
    });

    //Lists Connection


    //Radio Button Connection
    connect(ui->radioJE, &QRadioButton::clicked, this, [=](){
        Basic::edition = 0;
        ui->OriginEnchantment->refresh();
    });
    connect(ui->radioBE, &QRadioButton::clicked, this, [=](){
        Basic::edition = 1;
        ui->OriginEnchantment->refresh();
    });
    connect(ui->radioGA, &QRadioButton::clicked, this, [=](){
        Basic::mode[0] = 0;
    });
    connect(ui->radioG, &QRadioButton::clicked, this, [=](){
        Basic::mode[0] = 1;
    });
    connect(ui->radioE, &QRadioButton::clicked, this, [=](){
        Basic::mode[0] = 2;
    });
    connect(ui->radioEB, &QRadioButton::clicked, this, [=](){
        Basic::mode[1] = 0;
        ui->groupBox_4->setTitle("需求的魔咒（Needed Enchantment）");
        ui->NeededEnchantment->refresh();
    });
    connect(ui->radioAC, &QRadioButton::clicked, this, [=](){
        Basic::mode[1] = 1;
        ui->groupBox_4->setTitle("物品池（Item Pool）");
        ui->NeededEnchantment->refresh();
    });

    //Combo Box Connection
    connect(ui->InputItem, &QComboBox::currentIndexChanged, this, [=](){
        Basic::OriginItem.name = ui->InputItem->currentText();
        ui->OriginEnchantment->setWeapon(Basic::weapon[ui->InputItem->currentIndex()].name);
    });

    //Spin Box Connection
    connect(ui->Durability_0, &QSpinBox::valueChanged, this, [=](){
        Basic::OriginItem.duration = ui->Durability_0->value();
    });
    connect(ui->Penalty_0, &QSpinBox::valueChanged, this, [=](){
        Basic::OriginItem.penalty = ui->Penalty_0->value();
    });

    //Check Box Connection
    connect(ui->cbIgnoreFixing, &QCheckBox::clicked, this, [=](){
        Basic::lever[0] = ui->cbIgnoreFixing->isChecked();
    });
    connect(ui->cbIgnorePWP, &QCheckBox::clicked, this, [=](){
        Basic::lever[1] = ui->cbIgnorePWP->isChecked();
    });
    connect(ui->cbIgnoreCL, &QCheckBox::clicked, this, [=](){
        Basic::lever[2] = ui->cbIgnoreCL->isChecked();
    });

    //Push Button Connection
    connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->btnReset, &QPushButton::clicked, this, [=](){
        qApp->exit(5201314);
    });
    connect(ui->btnNext_1, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(1);
    });
    connect(ui->btnBack_2, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(0);
    });
    connect(ui->btnCalc_2, &QPushButton::clicked, this, [=](){
        if(ui->NeededEnchantment->enchCount() == 0)
        {
            ui->btnCalc_2->setStyleSheet("color:red");
            return;
        }
        else
            ui->btnCalc_2->setStyleSheet("color:black");

        QTime start_time = QTime::currentTime();

        for(int i = 0; i < INIT_LENGTH; i++)
            Basic::OriginItem.ench[i] = {};
        for(int i = 0; i < Basic::origin_ench_l; i++)
            Basic::OriginItem.ench[i] = Basic::origin_ench[i];

        Calculator();

        ui->OutputItem->setIcon(Basic::weapon[Basic::searchWeapon(Basic::OriginItem.name)].icon);
        ui->OutputItem->setIconSize(QSize(64, 64));
        ui->Durability_1->setText(QString::number(Basic::OutputItem.duration));
        ui->Penalty_1->setText(QString::number(Basic::OutputItem.penalty));
        ui->StepCount->setText(QString::number(Basic::flow_list_l));

        Basic::sumCost = 0;
        int step_max_cost = 0;
        for(int i = 0; i < Basic::flow_list_l; i++)
        {
            Basic::sumCost += Basic::flow_list[i].cost;
            step_max_cost = max(step_max_cost, Basic::flow_list[i].cost);
        }
        ui->CostLevel->setText(QString::number(Basic::sumCost));
        if(step_max_cost < 40)
        {
            ui->Notice->setText("可行的！\nAvailable!");
            ui->Notice->setStyleSheet("color:green");
        }
        else
        {
            ui->Notice->setText("过于昂贵！\nToo expensive!");
            ui->Notice->setStyleSheet("color:red");
        }

        ui->EnchantingFlow->refresh();
        ui->tabWidget->setCurrentIndex(2);

        QTime stop_time = QTime::currentTime();
        double num = (double)start_time.msecsTo(stop_time) / 1000;
        ui->CalcTime->setText(QString::number(num, 'g', 3) + "s");
    });
    connect(ui->btnBack_3, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(1);
    });
    connect(ui->btnSave, &QPushButton::clicked, this, [=](){
        FileOperate fo;
        fo.saveExport();
    });


    //Initialze & Load Files
    initialize();

    FileOperate fo;
    fo.loadConfig();
    if(Basic::config.default_edition == 0)
        ui->radioJE->setChecked(true);
    else
        ui->radioBE->setChecked(true);

    if(Basic::config.default_algorithm == 0)
        ui->radioGA->setChecked(true);
    else if(Basic::config.default_algorithm == 1)
        ui->radioG->setChecked(true);
    else
        ui->radioE->setChecked(true);

    if(Basic::config.enableCustomWe)
        fo.loadWeapon();
    if(Basic::config.enableCustomEn)
        fo.loadEnchantmentTable();

    if(Basic::config.autoCheckUpdate)
    {
        qDebug() << "autoCheckUpdate";
        CheckUpdate *task = new CheckUpdate();
        task->start(false);
    }

    refresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initialize()
{
    qDebug() << "Initializing...";
    Basic::weapon_l = 13;
    delete [] Basic::weapon;
    Basic::weapon = new Weapon[Basic::weapon_l];

    Basic::weapon[0] = {"剑\nSword",QIcon(":/icon/res/netherite_sword.png")};
    Basic::weapon[1] = {"镐\nPickaxe",QIcon(":/icon/res/netherite_pickaxe.png")};
    Basic::weapon[2] = {"斧\nAxe",QIcon(":/icon/res/netherite_axe.png")};
    Basic::weapon[3] = {"铲\nShovel",QIcon(":/icon/res/netherite_shovel.png")};
    Basic::weapon[4] = {"锄\nHoe",QIcon(":/icon/res/netherite_hoe.png")};
    Basic::weapon[5] = {"头盔\nHelmet",QIcon(":/icon/res/netherite_helmet.png")};
    Basic::weapon[6] = {"胸甲\nChestplate",QIcon(":/icon/res/netherite_chestplate.png")};
    Basic::weapon[7] = {"护腿\nLeggings",QIcon(":/icon/res/netherite_leggings.png")};
    Basic::weapon[8] = {"靴\nBoots",QIcon(":/icon/res/netherite_boots.png")};
    Basic::weapon[9] = {"弓\nBow",QIcon(":/icon/res/bow.png")};
    Basic::weapon[10] = {"弩\nCrossbow",QIcon(":/icon/res/crossbow_standby.png")};
    Basic::weapon[11] = {"三叉戟\nTrident",QIcon(":/icon/res/trident.png")};
    Basic::weapon[12] = {"钓鱼竿\nFishing Rod",QIcon(":/icon/res/fishing_rod.png")};


    Basic::ench_table_l = 38;
    delete [] Basic::ench_table;
    Basic::ench_table = new EnchTable[Basic::ench_table_l];

    Basic::ench_table[0] = {"水下速掘-aqua_affinity",1,{4,2},2,{},{0,0,0,0,0,1,0,0,0,0,0,0,0}};
    Basic::ench_table[1] = {"节肢杀手-bane_of_arthropods",5,{2,1},2,{"锋利-sharpness","亡灵杀手-smite"},{1,0,1,0,0,0,0,0,0,0,0,0,0}};
    Basic::ench_table[2] = {"爆炸保护-blast_protection",4,{4,2},2,{"火焰保护-fire_protection","弹射物保护-projectile_protection","保护-protection"},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    Basic::ench_table[3] = {"引雷-channeling",1,{8,4},2,{"激流-riptide"},{0,0,0,0,0,0,0,0,0,0,0,1,0}};
    Basic::ench_table[4] = {"绑定诅咒-binding_curse",1,{8,4},2,{},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    Basic::ench_table[5] = {"消失诅咒-vanishing_curse",1,{8,4},2,{},{1,1,1,1,1,1,1,1,1,1,1,1,1}};
    Basic::ench_table[6] = {"深海探索者-depth_strider",3,{4,2},2,{"冰霜行者-frost_walker"},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
    Basic::ench_table[7] = {"效率-efficiency",5,{1,1},2,{},{0,1,1,1,1,0,0,0,0,0,0,0,0}};
    Basic::ench_table[8] = {"摔落保护-feather_falling",4,{2,1},2,{},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
    Basic::ench_table[9] = {"火焰附加-fire_aspect",2,{4,2},2,{},{1,0,0,0,0,0,0,0,0,0,0,0,0}};
    Basic::ench_table[10] = {"火焰保护-fire_protection",4,{2,1},2,{"爆炸保护-blast_protection","弹射物保护-projectile_protection","保护-protection"},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    Basic::ench_table[11] = {"火矢-flame",1,{4,2},2,{},{0,0,0,0,0,0,0,0,0,1,0,0,0}};
    Basic::ench_table[12] = {"时运-fortune",3,{4,2},2,{"精准采集-silk_touch"},{0,1,1,1,1,0,0,0,0,0,0,0,0}};
    Basic::ench_table[13] = {"冰霜行者-frost_walker",2,{4,2},2,{"深海探索者-depth_strider"},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
    Basic::ench_table[14] = {"穿刺-impaling",5,{4,2},2,{},{0,0,0,0,0,0,0,0,0,0,0,1,0}};
    Basic::ench_table[15] = {"无限-infinity",1,{8,4},2,{"经验修补-mending"},{0,0,0,0,0,0,0,0,0,1,0,0,0}};
    Basic::ench_table[16] = {"击退-knockback",2,{2,1},2,{},{1,0,0,0,0,0,0,0,0,0,0,0,0}};
    Basic::ench_table[17] = {"抢夺-looting",3,{4,2},2,{},{1,0,0,0,0,0,0,0,0,0,0,0,0}};
    Basic::ench_table[18] = {"忠诚-loyalty",3,{1,1},2,{"激流-riptide"},{0,0,0,0,0,0,0,0,0,0,0,1,0}};
    Basic::ench_table[19] = {"海之眷顾-luck_of_the_sea",3,{4,2},2,{},{0,0,0,0,0,0,0,0,0,0,0,0,1}};
    Basic::ench_table[20] = {"饵钓-lure",3,{4,2},2,{},{0,0,0,0,0,0,0,0,0,0,0,0,1}};
    Basic::ench_table[21] = {"经验修补-mending",1,{4,2},2,{"无限-infinity"},{1,1,1,1,1,1,1,1,1,1,1,1,1}};
    Basic::ench_table[22] = {"多重射击-multishot",1,{4,2},2,{"穿透-piercing"},{0,0,0,0,0,0,0,0,0,0,1,0,0}};
    Basic::ench_table[23] = {"穿透-piercing",4,{1,1},2,{"多重射击-multishot"},{0,0,0,0,0,0,0,0,0,0,1,0,0}};
    Basic::ench_table[24] = {"力量-power",5,{1,1},2,{},{0,0,0,0,0,0,0,0,0,1,0,0,0}};
    Basic::ench_table[25] = {"弹射物保护-projectile_protection",4,{2,1},2,{"爆炸保护-blast_protection","火焰保护-fire_protection","保护-protection"},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    Basic::ench_table[26] = {"保护-protection",4,{1,1},2,{"爆炸保护-blast_protection","火焰保护-fire_protection","弹射物保护-projectile_protection"},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    Basic::ench_table[27] = {"冲击-punch",2,{4,2},2,{},{0,0,0,0,0,0,0,0,0,1,0,0,0}};
    Basic::ench_table[28] = {"快速装填-quick_charge",3,{2,1},2,{},{0,0,0,0,0,0,0,0,0,0,1,0,0}};
    Basic::ench_table[29] = {"水下呼吸-respiration",3,{4,2},2,{},{0,0,0,0,0,1,0,0,0,0,0,0,0}};
    Basic::ench_table[30] = {"激流-riptide",3,{4,2},2,{"引雷-channeling","忠诚-loyalty"},{0,0,0,0,0,0,0,0,0,0,0,1,0}};
    Basic::ench_table[31] = {"锋利-sharpness",5,{1,1},2,{"节肢杀手-bane_of_arthropods","亡灵杀手-smite"},{1,0,1,0,0,0,0,0,0,0,0,0,0}};
    Basic::ench_table[32] = {"精准采集-silk_touch",1,{8,4},2,{"时运-fortune"},{0,1,1,1,1,0,0,0,0,0,0,0,0}};
    Basic::ench_table[33] = {"亡灵杀手-smite",5,{2,1},2,{"节肢杀手-bane_of_arthropods","锋利-sharpness"},{1,0,1,0,0,0,0,0,0,0,0,0,0}};
    Basic::ench_table[34] = {"灵魂疾行-soul_speed",3,{8,4},2,{},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
    Basic::ench_table[35] = {"横扫之刃-sweeping",3,{4,2},0,{},{1,0,0,0,0,0,0,0,0,0,0,0,0}};
    Basic::ench_table[36] = {"荆棘-thorns",3,{8,4},2,{},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    Basic::ench_table[37] = {"耐久-unbreaking",3,{2,1},2,{},{1,1,1,1,1,1,1,1,1,1,1,1,1}};


    Basic::OriginItem.name = Basic::weapon[0].name;
    Basic::OriginItem.duration = 100;
    Basic::OriginItem.penalty = 0;
}

void MainWindow::refresh()
{
    ui->InputItem->clear();
    ui->InputItem->setIconSize(QSize(64,64));
    for(int i = 0; i < Basic::weapon_l; i++)
    {
        ui->InputItem->addItem(Basic::weapon[i].icon, Basic::weapon[i].name);
    }

    ui->OriginEnchantment->setWeapon(Basic::weapon[ui->InputItem->currentIndex()].name);
    ui->NeededEnchantment->setWeapon(Basic::weapon[ui->InputItem->currentIndex()].name);
}
