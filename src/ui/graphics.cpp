#include "graphics.h"
#include "ui_graphics.h"
#include <QApplication>

Graphics::Graphics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Graphics)
{
    ui->setupUi(this);
}

Graphics::~Graphics()
{
    delete ui;
}

int Graphics::run()
{
    qDebug() << "Graphics running";

    this->show();
    int ret = QApplication::exec();

    qDebug() << "Graphics exiting";
    return ret;
}
