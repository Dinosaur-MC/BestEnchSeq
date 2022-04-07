#include "settings.h"
#include "ui_settings.h"
#include "fileoperate.h"
#include "checkupdate.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    //Read Config
    if(DM->config.default_edition == MCE::Java)
        ui->radJE->setChecked(true);
    else
        ui->radBE->setChecked(true);

    if(DM->config.default_algorithm == ALGM::GlobalAverage)
        ui->radAlg_GA->setChecked(true);
    else if(DM->config.default_algorithm == ALGM::DifficultyFirst)
        ui->radAlg_DF->setChecked(true);
    else if(DM->config.default_algorithm == ALGM::Greedy)
        ui->radAlg_G->setChecked(true);
    else if(DM->config.default_algorithm == ALGM::Enumeration)
        ui->radAlg_E->setChecked(true);
    else if(DM->config.default_algorithm == ALGM::SimpleEnumeration)
        ui->radAlg_SE->setChecked(true);

    ui->cbConstant->setChecked(DM->config.constant);
    ui->leFilePath->setText(DM->config.export_path);
    ui->cbCustomWe->setChecked(DM->config.enableCustomWe);
    ui->cbCustomEn->setChecked(DM->config.enableCustomEn);
    ui->cbCheckUpate->setChecked(DM->config.autoCheckUpdate);

    //QPushButton Connections
    connect(ui->btnBrowse, &QPushButton::clicked, this, [=](){

    });

    connect(ui->btnCheckUpdate, &QPushButton::clicked, this, [=](){
        CheckUpdate *task = new CheckUpdate();
        ui->btnCheckUpdate->setEnabled(false);
        ui->checker->setText("正在检查更新...\nChecking for updates...");
        connect(task, &CheckUpdate::finished, ui->btnCheckUpdate, [=](){
            qDebug() << "Checking has been finished";
            ui->btnCheckUpdate->setEnabled(true);
            ui->checker->setText("");
        });
        task->start(true);
    });
    connect(ui->btnFeedback, &QPushButton::clicked, this, [=](){
        QDesktopServices::openUrl(QUrl("https://github.com/Dinosaur-MC/BestEnchSeq/issues"));
    });
    connect(ui->btnAbout, &QPushButton::clicked, this, [=](){
        QDialog w;
        QLabel *name = new QLabel(QString("* * * ") + PROGRAM_NAME_CN + " * * *\n* * * " + PROGRAM_NAME_EN + " * * *\n", &w);
        name->setAlignment(Qt::AlignHCenter);
        QLabel *ver = new QLabel(QString("Version: ") + VERSION, &w);
        ver->setAlignment(Qt::AlignHCenter);
        QLabel *ver_id = new QLabel(QString("Version ID: ") + QString::number(VERSION_ID), &w);
        ver_id->setAlignment(Qt::AlignHCenter);
        QLabel *author = new QLabel(QString("Author: ") + AUTHOR, &w);
        author->setAlignment(Qt::AlignHCenter);
        QLabel *web = new QLabel(QString("Web: ") + WEBSITE, &w);
        web->setAlignment(Qt::AlignHCenter);
        QPushButton *btn = new QPushButton("确定 Confirm", &w);
        connect(btn, &QPushButton::clicked, &w, &QDialog::accept);

        QVBoxLayout *layout = new QVBoxLayout(&w);
        layout->addWidget(name);
        layout->addWidget(ver);
        layout->addWidget(ver_id);
        layout->addWidget(author);
        layout->addWidget(web);
        layout->addWidget(btn);

        w.setFixedSize(360, 190);
        w.setLayout(layout);
        w.setModal(true);
        w.show();
        w.exec();
    });

    connect(ui->btnCancel, &QPushButton::clicked, this, &Settings::close);
    connect(ui->btnConfirm, &QPushButton::clicked, this, &Settings::accept);
    connect(ui->btnConfirm, &QPushButton::clicked, this, [=](){
        if(ui->radJE->isChecked())
            DM->config.default_edition = MCE::Java;
        else
            DM->config.default_edition = MCE::Bedrock;

        if(ui->radAlg_GA->isChecked())
            DM->config.default_algorithm = ALGM::GlobalAverage;
        else if(ui->radAlg_DF->isChecked())
            DM->config.default_algorithm = ALGM::DifficultyFirst;
        else if(ui->radAlg_G->isChecked())
            DM->config.default_algorithm = ALGM::Greedy;
        else if(ui->radAlg_E->isChecked())
            DM->config.default_algorithm = ALGM::Enumeration;
        else if(ui->radAlg_SE->isChecked())
            DM->config.default_algorithm = ALGM::SimpleEnumeration;

        DM->config.constant = ui->cbConstant->isChecked();
        DM->config.export_path = ui->leFilePath->text();

        DM->config.enableCustomWe = ui->cbCustomWe->isChecked();
        DM->config.enableCustomEn = ui->cbCustomEn->isChecked();
        DM->config.autoCheckUpdate = ui->cbCheckUpate->isChecked();

        FileOperate fo; //Question
        fo.saveConfig();
        if(DM->config.enableCustomWe)
            fo.loadWeapon();
        if(DM->config.enableCustomEn)
            fo.loadEnchantmentTable();
        close();
    });
}

Settings::~Settings()
{
    delete ui;
}
