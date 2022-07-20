#include "awindow.h"
#include "ui_awindow.h"
#include "qdebug.h"

AWindow::AWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AWindow)
{
    qDebug() << "[AWindow]";
    ui->setupUi(this);

}

AWindow::~AWindow()
{
    delete ui;
}


void AWindow::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);
}

void AWindow::keyReleaseEvent(QKeyEvent *e)
{
    QWidget::keyReleaseEvent(e);
}

