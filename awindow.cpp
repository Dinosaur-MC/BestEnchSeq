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


void loadInternalData()    // 加载内置数据
{

}

void initialize()    // 初始化
{

}

void refreshPage(int page = 0)    // 刷新页面列表
{

}


void initStatusBar()    // 设置状态栏
{

}

void onFirstLaunch()   // 首次启动时展示相关信息
{

}

void onUpdated()   // 软件更新后展示相关信息
{

}

void restart()  // 重启
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

