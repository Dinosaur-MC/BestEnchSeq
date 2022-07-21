#include "awindow.h"
#include "ui_awindow.h"

#include <QDebug>


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


void AWindow::loadInternalData()    // 加载内置数据
{

}

void AWindow::initialize()    // 初始化
{

}

void AWindow::refreshPage(int page)    // 刷新页面列表
{

}


void AWindow::initStatusBar()    // 设置状态栏
{

}

void AWindow::onFirstLaunch()   // 首次启动时展示相关信息
{

}

void AWindow::onUpdated()   // 软件更新后展示相关信息
{

}

void AWindow::restart()  // 重启程序
{

}



void AWindow::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);
}

void AWindow::keyReleaseEvent(QKeyEvent *e)
{
    QWidget::keyReleaseEvent(e);
}

