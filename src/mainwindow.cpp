#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("BESQ IV");
//    setWindowFlags(Qt::FramelessWindowHint);

    connect(ui->tb_close, &QToolButton::clicked, this, [=](){QApplication::exit(RESTART_CODE);});

}

MainWindow::~MainWindow()
{
    delete ui;
}
