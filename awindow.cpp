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

}


void AWindow::keyPressEvent(QKeyEvent *event)
{

}

void AWindow::keyReleaseEvent(QKeyEvent *event)
{

}

