#include "awindow.h"
#include "ui_awindow.h"

#include <QDebug>
#include <QFile>
#include "checkupdate.h"


AWindow::AWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AWindow)
{
    qDebug() << "[AWindow] (SETUP)";

    // 首次启动提示
    QFile fp(FILE_CONFIG);
    if(!fp.exists())
        onFirstLaunch();

    ui->setupUi(this);  // Setup ui
    this->setWindowTitle(QString(PROGRAM_NAME_CN) + " - " + VERSION);
    ui->tabWidget_ench->setTabVisible(1, false);

    initialize();   // 初始化

    if(cfg.deverloper_mode) // 开发者模式
        ui->menu_DeveloperTools->menuAction()->setVisible(true);
    else
        ui->menu_DeveloperTools->menuAction()->setVisible(false);

    if(cfg.auto_check_update)   // 启动时检查更新
    {
        CheckUpdate *cu = new CheckUpdate();
        connect(cu, &CheckUpdate::finished, this, [=](){
            int status = cu->status;
            if(status == 0)
                lb_update->setText("无更新 No upadate");
            else if(status == 1)
                lb_update->setText("发现更新 Found update!");
            else
                lb_update->setText("获取更新失败 Failed to get update");
        });

        cu->setUrl(QUrl(UPDATE_JSON));
        cu->start(false);
    }

    if(cfg.config_version < VERSION_ID) // 更新完成提示
    {
        onUpdated();
        cfg.config_version = VERSION_ID; // 更新配置版本号
        file_opr.saveConfig(cfg, &opt);
    }

    qDebug() << "[AWindow] Initialized.";
}

AWindow::~AWindow()
{
    delete ui;
    delete anv;
    delete e_filter;
    delete transf;
    delete lb_update;
    delete chml;
}


void AWindow::initialize()    // 初始化
{
    // 数据初始化
    pfaddn = 0b000;
    mce = MCE::Java;

    // 加载数据
    loadInternalData(&opt);
    loadInternalData(&cfg);
    file_opr.loadConfig(&cfg, &opt);

    if(cfg.enable_custom_we)
        file_opr.loadWeaponTable(&raw_weapon_table);
    else
        loadInternalData(&raw_weapon_table);

    if(cfg.enable_custom_en)
        file_opr.loadEnchantmentTable(&raw_enchantment_table);
    else
        loadInternalData(&raw_enchantment_table);

    // 分配数字 ID
    deliverID(&raw_weapon_table, &raw_enchantment_table, &weapon_table, &enchantment_table);

    // 初始化 Operator
    anv = new Anvil(&mce, &pfaddn, &enchantment_table);
    e_filter = new EnchFilter(&mce, &weapon_table, &enchantment_table);
    transf = new Transformer(&raw_weapon_table, &raw_enchantment_table);
    chml = new Chameleon(&raw_enchantment_table);

    // 配置状态栏
    initStatusBar();


    /* Connections */

    // 菜单项
    connect(ui->actionSettings, &QAction::triggered, this, [=](){   // 打开设置窗口
        Settings s;
        s.setModal(true);
        s.show();
        s.exec();
    });

    connect(ui->actionHelp, &QAction::triggered, this, [=](){   // 打开帮助窗口
        // Local helper
    });

    connect(ui->actionExit, &QAction::triggered, this, &exit);
    connect(ui->actionOpen_the_Editor, &QAction::triggered, this, [=](){
        // Table editor
    });

    connect(ui->actionEnable_Custom_Weapon, &QAction::triggered, this, [=](){
        cfg.enable_custom_we = ui->actionEnable_Custom_Weapon->isChecked();
    });

    connect(ui->actionEnable_Custom_Enchantment, &QAction::triggered, this, [=](){
        cfg.enable_custom_en = ui->actionEnable_Custom_Enchantment->isChecked();
    });

    connect(ui->actionWebsite, &QAction::triggered, this, [=](){
        QDesktopServices::openUrl(QUrl(WEBSITE));
    });

    connect(ui->actionVersion, &QAction::triggered, this, [=](){
        QDialog w;
        QLabel *name = new QLabel(QString("* * * ") + PROGRAM_NAME_CN + " * * *\n* * * " + PROGRAM_NAME_EN + " * * *\n", &w);
        name->setAlignment(Qt::AlignHCenter);
        QLabel *ver = new QLabel(QString("Version: ") + VERSION + " (" + QString::number(VERSION_ID) + ")", &w);
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

    connect(ui->actionUpdate, &QAction::triggered, this, [=](){
        CheckUpdate *cu = new CheckUpdate();
        connect(cu, &CheckUpdate::finished, this, [=](){
            int status = cu->status;
            if(status == 0)
                lb_update->setText("无更新 No upadate");
            else if(status == 1)
                lb_update->setText("发现更新 Found update!");
            else
                lb_update->setText("获取更新失败 Failed to get update");
        });

        cu->setUrl(QUrl(UPDATE_JSON));
        cu->start(true);
    });


    // Check Box
    connect(ui->cb_IgnorePenalty, &QCheckBox::clicked, this, [=](){
        if(ui->cb_IgnorePenalty->isChecked())
            pfaddn = pfaddn & ~0b100;
        else
            pfaddn = pfaddn | 0b100;
    });

    connect(ui->cb_IgnoreRepulsion, &QCheckBox::clicked, this, [=](){
        if(ui->cb_IgnoreRepulsion->isChecked())
            pfaddn = pfaddn & ~0b010;
        else
            pfaddn = pfaddn | 0b010;
    });

    connect(ui->cb_IgnoreFixing, &QCheckBox::clicked, this, [=](){
        if(ui->cb_IgnoreFixing->isChecked())
            pfaddn = pfaddn & ~0b001;
        else
            pfaddn = pfaddn | 0b001;
    });


    // Combo Box
    connect(ui->cb_InputItem, &QComboBox::currentIndexChanged, this, [=](){
        if(ui->cb_InputItem->currentIndex() >= 0)
        {
            raw_weapon = ui->cb_InputItem->currentWeapon();
            weapon = weapon_table.at(ui->cb_InputItem->currentIndex());
            current_item.type = weapon.id;
            aim_item.type = weapon.id;
            refreshPage(1);
        }
    });


    // Spin Box
    connect(ui->sb_Durability_0, &QSpinBox::valueChanged, this, [=](){
        current_item.durability = ui->sb_Durability_0->value();
    });

    connect(ui->sb_Penalty_0, &QSpinBox::valueChanged, this, [=](){
        current_item.penalty = ui->sb_Penalty_0->value();
    });


    // Push Button
    connect(ui->btnExit, &QPushButton::clicked, ui->actionExit, &QAction::triggered);
    connect(ui->btnReset, &QPushButton::clicked, this, &AWindow::restart);
    connect(ui->btnNext_1, &QPushButton::clicked, this, [=](){
        refreshPage(2);
        ui->tabWidget->setCurrentIndex(1);
    });

    connect(ui->btnNext_2, &QPushButton::clicked, this, [=](){
        if(ui->radioSI_AC->isChecked() && ui->tabWidget_ench->currentIndex() == 0) // 计算|下一页
            ui->tabWidget_ench->setCurrentIndex(1);
        else
        {
            // Calculator

            ui->tabWidget->setCurrentIndex(2);
        }
    });

    connect(ui->btnBack_2, &QPushButton::clicked, this, [=](){
        if(ui->radioSI_AC->isChecked() && ui->tabWidget_ench->currentIndex() == 1)
            ui->tabWidget_ench->setCurrentIndex(0);
        else
            ui->tabWidget->setCurrentIndex(0);
    });

    connect(ui->btnBack_3, &QPushButton::clicked, this, [=](){
        ui->tabWidget->setCurrentIndex(1);
    });

    connect(ui->btnSave, &QPushButton::clicked, this, [=](){
        file_opr.saveExport(cfg, raw_enchantment_table, summary, flow);
    });


    // Radio Button
    connect(ui->radioE_JE, &QRadioButton::toggled, this, [=](){
        if(ui->radioE_JE->isChecked())
            mce = MCE::Java;
        else
            mce = MCE::Bedrock;

        refreshPage(0);
    });

    connect(ui->radioSI_ALEB, &QRadioButton::clicked, this, [=](){
        icm = ICM::AllLevelEBook;
    });

    connect(ui->radioSI_EB, &QRadioButton::clicked, this, [=](){
        icm = ICM::BasicEBook;
    });

    connect(ui->radioSI_AC, &QRadioButton::toggled, this, [=](){
        if(ui->radioSI_AC->isChecked()) // 高级物品配置模式
        {
            icm = ICM::AdvanceMode;
            ui->tabWidget_ench->setCurrentIndex(0);
            ui->tabWidget_ench->setTabVisible(1, true);
            ui->btnNext_2->setText("下一步（Next）");
        }
        else
        {
            ui->tabWidget_ench->setCurrentIndex(0);
            ui->tabWidget_ench->setTabVisible(1, false);
            ui->btnNext_2->setText("计 算（Calculate）");
        }
    });

    connect(ui->radioA_GA, &QRadioButton::clicked, this, [=](){
        alg = ALGM::GlobalAverage;
    });

    connect(ui->radioA_DF, &QRadioButton::clicked, this, [=](){
        alg = ALGM::DifficultyFirst;
    });

    connect(ui->radioA_G, &QRadioButton::clicked, this, [=](){
        alg = ALGM::Greedy;
    });

    connect(ui->radioA_E, &QRadioButton::clicked, this, [=](){
        alg = ALGM::Enumeration;
    });

    connect(ui->radioA_SE, &QRadioButton::clicked, this, [=](){
        alg = ALGM::SimpleEnumeration;
    });

    // Tab Widget
    connect(ui->tabWidget_ench, &QTabWidget::currentChanged, [=]{
        if(ui->tabWidget_ench->currentIndex() == 0)
            ui->btnNext_2->setText("下一步（Next）");
        else
            ui->btnNext_2->setText("计 算（Calculate）");
    });

    /* Connections */

    // 容器初始化
    ui->ListOriginEnchantment->setMode(0);
    ui->ListNeededEnchantment->setMode(1);
    e_filter->setBase(&current_item.ench);

    refreshPage(0);

}

void AWindow::refreshPage(int page)    // 刷新页面列表
{
    if(page == 0)
    {
        ui->cb_InputItem->reload(raw_weapon_table);
        raw_weapon = ui->cb_InputItem->currentWeapon();
        weapon = weapon_table.at(ui->cb_InputItem->currentIndex());
        current_item.type = weapon.id;
        aim_item.type = weapon.id;
        e_filter->setWeapon(weapon);

        refreshPage(3);
        refreshPage(1);
        refreshPage(2);
    }
    else if(page == 1)
    {
        qDebug() << "ui->cb_InputItem->currentIndex() " << weapon_table.count() << ui->cb_InputItem->currentIndex();
        weapon = weapon_table.at(ui->cb_InputItem->currentIndex());
        if(chml->fromVEnchPlus(weapon.suitableE) != NULL)
            ui->ListOriginEnchantment->reload(chml->toVEnchPro());
    }
    else if(page == 2)
    {
        current_item.ench = ui->ListOriginEnchantment->getCheckedItem();
        if(chml->fromVEnch(e_filter->getEnchSet()) != NULL)
            ui->ListNeededEnchantment->reload(chml->toVEnchPro());
//        ui->ListItemPool->clear();
    }
    else if(page == 3)
    {
        ui->label_Durability_1->setText("---");
        ui->label_Penalty_1->setText("---");
        ui->label_LevelCost->setText("---");
        ui->label_PointCost->setText("---");
        ui->label_StepCount->setText("---");
        ui->label_CalcTime->setText("------");
        ui->label_Notice->setText("无结果\nNo Result");
        if(flow.count() > 0)
        {
            QVector<FlowStepPro> fsps;
            for(int i = 0; i < flow.count(); i++)
                fsps.append(transf->toFlowStepPro(&flow.at(i)));
            ui->ListEnchantingFlow->reload(flow, fsps);
        }
    }
}


void AWindow::initStatusBar()    // 设置状态栏
{
    lb_update = new QLabel(this);
    ui->statusBar->addPermanentWidget(lb_update);
}

void AWindow::onFirstLaunch()   // 首次启动时展示相关信息
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

void AWindow::onUpdated()   // 软件更新后展示相关信息
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

void AWindow::restart()  // 重启程序
{
    QApplication::exit(RESTART_CODE);
}



void AWindow::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);
}

void AWindow::keyReleaseEvent(QKeyEvent *e)
{
    QWidget::keyReleaseEvent(e);
}

