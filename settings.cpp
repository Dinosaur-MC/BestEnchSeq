#include "settings.h"
#include "ui_settings.h"
#include "basic.h"
#include "fileoperate.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

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

    connect(ui->btnBrowse, &QPushButton::clicked, this, [=](){});
    connect(ui->btnCheckUpdate, &QPushButton::clicked, this, [=](){});
    connect(ui->btnFeedback, &QPushButton::clicked, this, [=](){});
    connect(ui->btnAbout, &QPushButton::clicked, this, [=](){});

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
