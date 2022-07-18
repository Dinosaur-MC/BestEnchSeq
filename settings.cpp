#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog{parent},
    ui(new Ui::Settings)
{

}

Settings::~Settings()
{
    delete ui;
}
