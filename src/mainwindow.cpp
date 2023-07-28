#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/fileoperator.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("BESQ IV");
//    setWindowFlags(Qt::FramelessWindowHint);

    connect(ui->tb_close, &QToolButton::clicked, this, &MainWindow::close);

}

MainWindow::~MainWindow()
{
    delete ui;
}
