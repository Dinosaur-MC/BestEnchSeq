#include "waitwidget.h"
#include "ui_waitwidget.h"
#include <QPainter>

WaitWidget::WaitWidget(QWidget *parent):
    QDialog{parent},
    ui(new Ui::WaitWidget)
{
    ui->setupUi(this);
    st = QTime::currentTime();
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &WaitWidget::update);
    timer->start(500);
}

WaitWidget::~WaitWidget()
{
    delete ui;
}

void WaitWidget::update()
{
    qDebug() << "[Calc Progress] " << DM->calcProgress;
    ui->progressBar->setMaximum(DM->complexity);
    ui->progressBar->setValue(DM->calcProgress);
    et = {0,0,0,0};
    et = et.addSecs(st.secsTo(QTime::currentTime()));
    ui->time->setText("用时(Cost Time)：" + et.toString("hh:mm:ss"));
}

void WaitWidget::Done()
{
    qDebug() << "WaitWidget: Done";
    timer->stop();
    close();
}




WaitWidgetScreen::WaitWidgetScreen(QWidget *parent):
    QFrame{parent}
{
}

WaitWidgetScreen::~WaitWidgetScreen()
{

}

void WaitWidgetScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pm(":/icon/res/logo.png");
    if(width() > height())
        painter.drawPixmap(width()/2 - height()/2, 0, height(), height(), pm);
    else
        painter.drawPixmap(0, height()/2 - width()/2, width(), width(), pm);
}

