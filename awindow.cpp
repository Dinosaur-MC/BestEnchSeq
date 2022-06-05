#include "AWindow.h"
#include "ui_AWindow.h"

AWindow::AWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AWindow)
{
    qDebug() << "[AWindow]";
    ui->setupUi(this);

}
