#include "waitwidget.h"
#include "ui_waitwidget.h"

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

