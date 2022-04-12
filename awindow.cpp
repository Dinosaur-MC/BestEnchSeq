#include "awindow.h"
#include "ui_awindow.h"
#include <QFile>
#include "fileoperate.h"
#include "settings.h"
#include "checkupdate.h"
#include "calculator.h"

AWindow::AWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AWindow)
{
    qDebug() << "[AWindow]";
    ui->setupUi(this);
    ui->menu_DebugTools->menuAction()->setVisible(false);
    setWindowTitle(QString(PROGRAM_NAME_CN) + PROGRAM_NAME_EN + " - " + VERSION);
    setStatusBarText();

    FileOperate fo;
    fo.loadConfig();
    loadInternalData();

    if(DM->config.enableCustomEn)
        fo.loadEnchantmentTable();
    if(DM->config.enableCustomWe)
        fo.loadWeapon();

    if(DM->isFirstLaunch)
    {
        onFirstLaunch();
        DM->isFirstLaunch = false;
    }

    init();
    refreshPage(1);

    if(DM->isUpdated)
    {
        onUpdated();
        DM->config.config_version = FILEVERSION;
        DM->isUpdated = false;
        fo.saveConfig();
    }
    if(DM->config.autoCheckUpdate)
    {
        CheckUpdate *cu = new CheckUpdate();
        cu->start(0);
    }
}

AWindow::~AWindow()
{
    delete ui;
}


void AWindow::loadInternalData()    //加载内部数据，自定义模式关闭时生效
{
    DM->weapon_l = 13;
    delete [] DM->weapon;
    DM->weapon = new Weapon[DM->weapon_l];

    DM->weapon[0] = {"剑\nSword",QIcon(":/icon/res/netherite_sword.png")};
    DM->weapon[1] = {"镐\nPickaxe",QIcon(":/icon/res/netherite_pickaxe.png")};
    DM->weapon[2] = {"斧\nAxe",QIcon(":/icon/res/netherite_axe.png")};
    DM->weapon[3] = {"铲\nShovel",QIcon(":/icon/res/netherite_shovel.png")};
    DM->weapon[4] = {"锄\nHoe",QIcon(":/icon/res/netherite_hoe.png")};
    DM->weapon[5] = {"头盔\nHelmet",QIcon(":/icon/res/netherite_helmet.png")};
    DM->weapon[6] = {"胸甲\nChestplate",QIcon(":/icon/res/netherite_chestplate.png")};
    DM->weapon[7] = {"护腿\nLeggings",QIcon(":/icon/res/netherite_leggings.png")};
    DM->weapon[8] = {"靴\nBoots",QIcon(":/icon/res/netherite_boots.png")};
    DM->weapon[9] = {"弓\nBow",QIcon(":/icon/res/bow.png")};
    DM->weapon[10] = {"弩\nCrossbow",QIcon(":/icon/res/crossbow_standby.png")};
    DM->weapon[11] = {"三叉戟\nTrident",QIcon(":/icon/res/trident.png")};
    DM->weapon[12] = {"钓鱼竿\nFishing Rod",QIcon(":/icon/res/fishing_rod.png")};


    DM->ench_table_l = 39;
    delete [] DM->ench_table;
    DM->ench_table = new EnchTable[DM->ench_table_l];

    DM->ench_table[0] = {"水下速掘-aqua_affinity",1,1,{4,2},MCE::All,{""},{0,0,0,0,0,1,0,0,0,0,0,0,0}};
    DM->ench_table[1] = {"节肢杀手-bane_of_arthropods",4,5,{2,1},MCE::All,{"锋利-sharpness","亡灵杀手-smite"},{1,0,1,0,0,0,0,0,0,0,0,0,0}};
    DM->ench_table[2] = {"爆炸保护-blast_protection",4,4,{4,2},MCE::All,{"火焰保护-fire_protection","弹射物保护-projectile_protection","保护-protection"},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    DM->ench_table[3] = {"引雷-channeling",1,1,{8,4},MCE::All,{"激流-riptide"},{0,0,0,0,0,0,0,0,0,0,0,1,0}};
    DM->ench_table[4] = {"绑定诅咒-binding_curse",0,1,{8,4},MCE::All,{""},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    DM->ench_table[5] = {"消失诅咒-vanishing_curse",0,1,{8,4},MCE::All,{""},{1,1,1,1,1,1,1,1,1,1,1,1,1}};
    DM->ench_table[6] = {"深海探索者-depth_strider",3,3,{4,2},MCE::All,{"冰霜行者-frost_walker"},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
    DM->ench_table[7] = {"效率-efficiency",4,5,{1,1},MCE::All,{""},{0,1,1,1,1,0,0,0,0,0,0,0,0}};
    DM->ench_table[8] = {"摔落保护-feather_falling",4,4,{2,1},MCE::All,{""},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
    DM->ench_table[9] = {"火焰附加-fire_aspect",2,2,{4,2},MCE::All,{""},{1,0,0,0,0,0,0,0,0,0,0,0,0}};
    DM->ench_table[10] = {"火焰保护-fire_protection",4,4,{2,1},MCE::All,{"爆炸保护-blast_protection","弹射物保护-projectile_protection","保护-protection"},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    DM->ench_table[11] = {"火矢-flame",1,1,{4,2},MCE::All,{""},{0,0,0,0,0,0,0,0,0,1,0,0,0}};
    DM->ench_table[12] = {"时运-fortune",3,3,{4,2},MCE::All,{"精准采集-silk_touch"},{0,1,1,1,1,0,0,0,0,0,0,0,0}};
    DM->ench_table[13] = {"冰霜行者-frost_walker",0,2,{4,2},MCE::All,{"深海探索者-depth_strider"},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
    DM->ench_table[14] = {"穿刺-impaling",5,5,{4,2},MCE::All,{""},{0,0,0,0,0,0,0,0,0,0,0,1,0}};
    DM->ench_table[15] = {"无限-infinity",1,1,{8,4},MCE::All,{"经验修补-mending"},{0,0,0,0,0,0,0,0,0,1,0,0,0}};
    DM->ench_table[16] = {"击退-knockback",2,2,{2,1},MCE::All,{""},{1,0,0,0,0,0,0,0,0,0,0,0,0}};
    DM->ench_table[17] = {"抢夺-looting",3,3,{4,2},MCE::All,{""},{1,0,0,0,0,0,0,0,0,0,0,0,0}};
    DM->ench_table[18] = {"忠诚-loyalty",3,3,{1,1},MCE::All,{"激流-riptide"},{0,0,0,0,0,0,0,0,0,0,0,1,0}};
    DM->ench_table[19] = {"海之眷顾-luck_of_the_sea",3,3,{4,2},MCE::All,{""},{0,0,0,0,0,0,0,0,0,0,0,0,1}};
    DM->ench_table[20] = {"饵钓-lure",3,3,{4,2},MCE::All,{""},{0,0,0,0,0,0,0,0,0,0,0,0,1}};
    DM->ench_table[21] = {"经验修补-mending",0,1,{4,2},MCE::All,{"无限-infinity"},{1,1,1,1,1,1,1,1,1,1,1,1,1}};
    DM->ench_table[22] = {"多重射击-multishot",1,1,{4,2},MCE::All,{"穿透-piercing"},{0,0,0,0,0,0,0,0,0,0,1,0,0}};
    DM->ench_table[23] = {"穿透-piercing",4,4,{1,1},MCE::All,{"多重射击-multishot"},{0,0,0,0,0,0,0,0,0,0,1,0,0}};
    DM->ench_table[24] = {"力量-power",4,5,{1,1},MCE::All,{""},{0,0,0,0,0,0,0,0,0,1,0,0,0}};
    DM->ench_table[25] = {"弹射物保护-projectile_protection",4,4,{2,1},MCE::All,{"爆炸保护-blast_protection","火焰保护-fire_protection","保护-protection"},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    DM->ench_table[26] = {"保护-protection",4,4,{1,1},MCE::All,{"爆炸保护-blast_protection","火焰保护-fire_protection","弹射物保护-projectile_protection"},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    DM->ench_table[27] = {"冲击-punch",2,2,{4,2},MCE::All,{""},{0,0,0,0,0,0,0,0,0,1,0,0,0}};
    DM->ench_table[28] = {"快速装填-quick_charge",3,3,{2,1},MCE::All,{""},{0,0,0,0,0,0,0,0,0,0,1,0,0}};
    DM->ench_table[29] = {"水下呼吸-respiration",3,3,{4,2},MCE::All,{""},{0,0,0,0,0,1,0,0,0,0,0,0,0}};
    DM->ench_table[30] = {"激流-riptide",3,3,{4,2},MCE::All,{"引雷-channeling","忠诚-loyalty"},{0,0,0,0,0,0,0,0,0,0,0,1,0}};
    DM->ench_table[31] = {"锋利-sharpness",4,5,{1,1},MCE::All,{"节肢杀手-bane_of_arthropods","亡灵杀手-smite"},{1,0,1,0,0,0,0,0,0,0,0,0,0}};
    DM->ench_table[32] = {"精准采集-silk_touch",1,1,{8,4},MCE::All,{"时运-fortune"},{0,1,1,1,1,0,0,0,0,0,0,0,0}};
    DM->ench_table[33] = {"亡灵杀手-smite",4,5,{2,1},MCE::All,{"节肢杀手-bane_of_arthropods","锋利-sharpness"},{1,0,1,0,0,0,0,0,0,0,0,0,0}};
    DM->ench_table[34] = {"灵魂疾行-soul_speed",0,3,{8,4},MCE::All,{""},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
    DM->ench_table[35] = {"横扫之刃-sweeping",3,3,{4,2},MCE::Java,{""},{1,0,0,0,0,0,0,0,0,0,0,0,0}};
    DM->ench_table[36] = {"荆棘-thorns",3,3,{8,4},MCE::All,{""},{0,0,0,0,0,1,1,1,1,0,0,0,0}};
    DM->ench_table[37] = {"耐久-unbreaking",3,3,{2,1},MCE::All,{""},{1,1,1,1,1,1,1,1,1,1,1,1,1}};
    DM->ench_table[38] = {"迅捷潜行_swift_sneak",0,3,{8,4},MCE::Java,{""},{0,0,0,0,0,0,0,0,1,0,0,0,0}};
}

void AWindow::init()
{
    //************ Default Values ************
    //- - - - - - - - Page 1 - - - - - - - -
    ui->cb_InputItem->clear();
    for(int i = 0; i < DM->weapon_l; i++)
    {
        ui->cb_InputItem->addItem(DM->weapon[i].name);
        ui->cb_InputItem->setItemIcon(i, DM->weapon[i].icon);
    }
    DM->OriginItem->name = DM->weapon[0].name;
    DM->OriginItem->durability = 100;
    DM->OriginItem->penalty = 0;

    ui->ListOriginEnchantment->setOutputAddress(DM->origin_ench, DM->resizeOriginEnchList);
    ui->ListNeededEnchantment->setOutputAddress(DM->needed_ench, DM->resizeNeededEnchList);

    //- - - - - - - - Page 2 - - - - - - - -
    ui->tabWidget_ench->tabBar()->setHidden(true);
    ui->tab_IP->setHidden(true);

    //- - - - - - - - Page 3 - - - - - - - -
    ui->tabWidget_flow->tabBar()->setHidden(true);

    //************ Default Values End ************


    //************ Connections ************
    //- - - - - - - - Menu Bar - - - - - - - -
    connect(ui->actionSettings, &QAction::triggered, this, [=](){
        Settings *w = new Settings();
        w->setModal(true);
        w->show();
        if(w->exec() == Settings::Accepted)
        {
            restart();
        }
    });
    connect(ui->actionHelp, &QAction::triggered, this, [=](){});
    connect(ui->actionExit, &QAction::triggered, this, &AWindow::close);

    connect(ui->actionEnchantment, &QAction::triggered, this, [=](){});
    connect(ui->actionWeapon, &QAction::triggered, this, [=](){});

    connect(ui->actionWebsite, &QAction::triggered, this, [=](){});
    connect(ui->actionVersion, &QAction::triggered, this, [=](){});
    connect(ui->actionUpdate, &QAction::triggered, this, [=](){});

    //- - - - - - - - Page 1 - - - - - - - -
    //Edition Selection
    connect(ui->radioE_JE, &QRadioButton::clicked, this, [=](){
        if(DM->edition == MCE::Java)
            return;
        DM->edition = MCE::Java;
        refreshPage(1);
    });
    connect(ui->radioE_BE, &QRadioButton::clicked, this, [=](){
        if(DM->edition == MCE::Bedrock)
            return;
        DM->edition = MCE::Bedrock;
        refreshPage(1);
    });

    //Choose weapon kind & set basic info.
    connect(ui->cb_InputItem, &QComboBox::currentIndexChanged, this, [=](){
        DM->OriginItem->name = ui->cb_InputItem->currentText();
        refreshPage(1);
    });
    connect(ui->sb_Durability_0, &QSpinBox::valueChanged, this, [=](){
        DM->OriginItem->durability = ui->sb_Durability_0->value();
    });
    connect(ui->sb_Penalty_0, &QSpinBox::valueChanged, this, [=](){
        DM->OriginItem->penalty = ui->sb_Penalty_0->value();
    });

    //Buttons
    connect(ui->btnExit, &QPushButton::clicked, this, &AWindow::close);
    connect(ui->btnNext_1, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(1);
        refreshPage(2);
    });
    connect(ui->btnReset, &QPushButton::clicked, this, [=](){
        restart();
    });

    //- - - - - - - - Page 2 - - - - - - - -
    //Item Config Selection
    connect(ui->radioSI_ALEB, &QRadioButton::clicked, this, [=](){
        if(DM->itemconfig == ICM::AllLevelEBook)
            return;
        DM->itemconfig = ICM::AllLevelEBook;
        ui->groupBox_Item->setTitle("需求的魔咒（Needed Enchantment）");
        ui->btnNext_2->setText("计 算（Calculate）");
        ui->tabWidget_ench->setCurrentIndex(0);
        ui->tabWidget_ench->tabBar()->setHidden(true);
        ui->tab_IP->setHidden(true);
        refreshPage(2);
    });
    connect(ui->radioSI_EB, &QRadioButton::clicked, this, [=](){
        if(DM->itemconfig == ICM::BasicEBook)
            return;
        DM->itemconfig = ICM::BasicEBook;
        ui->groupBox_Item->setTitle("需求的魔咒（Needed Enchantment）");
        ui->btnNext_2->setText("计 算（Calculate）");
        ui->tabWidget_ench->setCurrentIndex(0);
        ui->tabWidget_ench->tabBar()->setHidden(true);
        ui->tab_IP->setHidden(true);
        refreshPage(2);
    });
    connect(ui->radioSI_AC, &QRadioButton::clicked, this, [=](){
        if(DM->itemconfig == ICM::AdvanceMode)
            return;
        DM->itemconfig = ICM::AdvanceMode;
        ui->groupBox_Item->setTitle("物品配置（Item Configuration）");
        ui->btnNext_2->setText("下一步（Next）");
        ui->tabWidget_ench->tabBar()->setHidden(false);
        ui->tab_IP->setHidden(false);
        refreshPage(2);
    });

    //Algorithm Selection
    connect(ui->radioA_GA, &QRadioButton::clicked, this, [=](){
        if(DM->alg_mode != ALGM::GlobalAverage)
            DM->alg_mode = ALGM::GlobalAverage;
    });
    connect(ui->radioA_DF, &QRadioButton::clicked, this, [=](){
        if(DM->alg_mode != ALGM::DifficultyFirst)
            DM->alg_mode = ALGM::DifficultyFirst;
    });
    connect(ui->radioA_G, &QRadioButton::clicked, this, [=](){
        if(DM->alg_mode != ALGM::Greedy)
            DM->alg_mode = ALGM::Greedy;
    });
    connect(ui->radioA_E, &QRadioButton::clicked, this, [=](){
        if(DM->alg_mode != ALGM::Enumeration)
            DM->alg_mode = ALGM::Enumeration;
    });
    connect(ui->radioA_SE, &QRadioButton::clicked, this, [=](){
        if(DM->alg_mode != ALGM::SimpleEnumeration)
            DM->alg_mode = ALGM::SimpleEnumeration;
    });

    //Additions
    connect(ui->cb_IgnoreFixing, &QCheckBox::clicked, this, [=](){
        DM->addition[0] = ui->cb_IgnoreFixing->isChecked();
    });
    connect(ui->cb_IgnorePenalty, &QCheckBox::clicked, this, [=](){
        DM->addition[1] = ui->cb_IgnorePenalty->isChecked();
    });
    connect(ui->cb_IgnoreCL, &QCheckBox::clicked, this, [=](){
        DM->addition[2] = ui->cb_IgnoreCL->isChecked();
    });

    connect(ui->tabWidget_ench, &QTabWidget::currentChanged, this, [=](){
        if(ui->tabWidget_ench->currentIndex() == 0)
            ui->btnNext_2->setText("下一步（Next）");
        else
            ui->btnNext_2->setText("计 算（Calculate）");
    });

    //Buttons
    connect(ui->btnBack_2, &QPushButton::clicked, this, [=](){
        if(DM->itemconfig == ICM::AdvanceMode && ui->tabWidget_ench->currentIndex() == 1)
        {
            ui->tabWidget_ench->setCurrentIndex(0);
            ui->btnNext_2->setText("下一步（Next）");
        }
        else
            ui->tabWidget->setCurrentIndex(0);
    });
    connect(ui->btnNext_2, &QPushButton::clicked, this, [=](){
        if(DM->itemconfig == ICM::AdvanceMode && ui->tabWidget_ench->currentIndex() == 0)
        {
            ui->tabWidget_ench->setCurrentIndex(1);
            ui->btnNext_2->setText("计 算（Calculate）");
        }
        else
        {
            if(ui->ListNeededEnchantment->enchCount() == 0)
            {
                return;
            }
            ui->tabWidget->setCurrentIndex(2);
            //Calculator();
            refreshPage(3);
        }
    });

    //- - - - - - - - Page 3 - - - - - - - -
    //Buttons
    connect(ui->btnBack_3, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(1);
    });
    connect(ui->btnSave, &QPushButton::clicked, this, [=](){
        FileOperate fo;
        fo.saveExport();
    });

    //************ Connections End ************
}

void AWindow::refreshPage(int page) //刷新页面列表 (page; 0:Reflush all, 1:Reflush page 1 and clean other pages, 2:Reflush page 2, 3:Reflush Page 3)
{
    if(page == 0)
    {
        ui->ListEnchantingFlow->clear();
        ui->ListItemPool->reinit();
        ui->ListOriginEnchantment->setWeapon(DM->OriginItem->name);
        ui->ListNeededEnchantment->setPlate(DM->origin_ench, DM->origin_ench_l);
        ui->ListNeededEnchantment->setWeapon(DM->OriginItem->name);
    }
    if(page == 1)
    {
        ui->ListOriginEnchantment->setWeapon(DM->OriginItem->name);
        ui->ListNeededEnchantment->clear();
        ui->ListItemPool->reinit();
    }
    if(page == 2)
    {
        ui->ListNeededEnchantment->setPlate(DM->origin_ench, DM->origin_ench_l);
        ui->ListNeededEnchantment->setWeapon(DM->OriginItem->name);
        ui->ListItemPool->reinit();
    }
    if(page == 3)
    {
        ui->ListEnchantingFlow->refresh();
    }
}


void AWindow::setStatusBarText()
{
    QLabel *label = new QLabel(QString("Website: ") + WEBSITE);
    ui->statusBar->addWidget(label);
}

void AWindow::onFirstLaunch()
{
    QDialog *w = new QDialog();
    w->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    w->setWindowTitle("欢迎使用 Welcome to use");
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QFont ft;
    ft.setPointSize(20);
    QLabel *title = new QLabel(w);
    title->setFont(ft);
    title->setAlignment(Qt::AlignCenter);
    title->setText(QString("") + PROGRAM_NAME_CN + " " + VERSION + "\n" + PROGRAM_NAME_EN);

    QFont ft2;
    ft2.setPointSize(12);
    QLabel *text = new QLabel(w);
    text->setFont(ft2);
    text->setWordWrap(true);
    text->setText(QString("\n【声明】") + STATEMENT + "\n[STATEMENT] " + STATEMENT_EN + "\n\n");

    QPushButton *btnY = new QPushButton(w);
    connect(btnY, &QPushButton::clicked, w, &QDialog::accept);
    btnY->setText("I Know");
    QPushButton *btnN = new QPushButton(w);
    connect(btnN, &QPushButton::clicked, w, &QDialog::reject);
    connect(w, &QDialog::rejected, this, [=](){
        QFile::remove(FILE_CONFIG);
        exit(0);
    });
    btnN->setText("Close");

    layout->addWidget(title);
    layout->addWidget(text);
    layout->addWidget(btnY);
    layout->addWidget(btnN);
    w->setLayout(layout);
    w->show();
    w->setModal(true);
    w->exec();
}

void AWindow::onUpdated()
{
    QDialog *w = new QDialog();
    w->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    w->setWindowTitle("提示 Notice");
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QFont ft;
    ft.setPointSize(12);
    QLabel *title = new QLabel(w);
    title->setFont(ft);
    title->setAlignment(Qt::AlignCenter);
    title->setText(QString("") + PROGRAM_NAME_CN + " " + VERSION + "\n" + PROGRAM_NAME_EN);

    QFont ft2;
    ft2.setPointSize(10);
    QLabel *text = new QLabel(w);
    text->setFont(ft2);
    text->setWordWrap(true);
    text->setText(QString("\n软件已升级至 ") + VERSION + " ！\n" + "Updated to version " + VERSION + " successfully!\n");

    QPushButton *btn = new QPushButton(w);
    connect(btn, &QPushButton::clicked, w, &QDialog::accept);
    btn->setText("Yes");

    layout->addWidget(title);
    layout->addWidget(text);
    layout->addWidget(btn);
    w->setLayout(layout);
    w->show();
    w->setModal(true);
    w->exec();
}

void AWindow::restart()
{
    qApp->exit(RESTART_CODE);
}


void AWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_M)
        {
            if(event->isAutoRepeat())
                return;
            qDebug() << "Pressed [Ctrl + M]";
            ui->menu_DebugTools->menuAction()->setVisible(!ui->menu_DebugTools->menuAction()->isVisible());
        }
    }
    else
        QWidget::keyPressEvent(event);
}

void AWindow::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
}

