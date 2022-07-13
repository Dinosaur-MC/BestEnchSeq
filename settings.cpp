#include "settings.h"
#include "ui_settings.h"
#include "basic.h"
#include "fileoperate.h"
#include "checkupdate.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    //Read Config
    if(Basic::config.default_edition == 0)
        ui->radJE->setChecked(true);
    else
        ui->radBE->setChecked(true);

    if(Basic::config.default_algorithm == 0)
        ui->radAlgDF->setChecked(true);
    else if(Basic::config.default_algorithm == 1)
        ui->radAlgGr->setChecked(true);
    else if(Basic::config.default_algorithm == 2)
        ui->radAlgEn->setChecked(true);

    ui->cbConstant->setChecked(Basic::config.constant);
    ui->leFilePath->setText(Basic::config.export_path);
    ui->cbCustomWe->setChecked(Basic::config.enableCustomWe);
    ui->cbCustomEn->setChecked(Basic::config.enableCustomEn);
    ui->cbCheckUpate->setChecked(Basic::config.autoCheckUpdate);
    ui->cbERW->setChecked(Basic::config.enableReszieWindow);

    //QPushButton Connections
    connect(ui->btnBrowse, &QPushButton::clicked, this, [=](){

    });

    connect(ui->btnCheckUpdate, &QPushButton::clicked, this, [=](){
        CheckUpdate *task = new CheckUpdate();
        ui->btnCheckUpdate->setEnabled(false);
        ui->checker->setText("正在检查更新...\nScanning update...");
        connect(task, &CheckUpdate::finished, ui->btnCheckUpdate, [=](){
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
            Basic::config.default_edition = 0;
        else
            Basic::config.default_edition = 1;

        if(ui->radAlgDF->isChecked())
            Basic::config.default_algorithm = 0;
        else if(ui->radAlgGr->isChecked())
            Basic::config.default_algorithm = 1;
        else if(ui->radAlgEn->isChecked())
            Basic::config.default_algorithm = 2;

        Basic::config.constant = ui->cbConstant->isChecked();
        Basic::config.export_path = ui->leFilePath->text();

        Basic::config.enableCustomWe = ui->cbCustomWe->isChecked();
        Basic::config.enableCustomEn = ui->cbCustomEn->isChecked();
        Basic::config.autoCheckUpdate = ui->cbCheckUpate->isChecked();

        Basic::config.enableReszieWindow = ui->cbERW->isChecked();

        FileOperate fo;
        fo.saveConfig();
        if(Basic::config.enableCustomWe)
            fo.loadWeapon();
        if(Basic::config.enableCustomEn)
            fo.loadEnchantmentTable();
        close();
    });
}

Settings::~Settings()
{
    delete ui;
}
