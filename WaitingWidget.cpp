#include "waitingwidget.h"
#include "ui_waitingwidget.h"


WaitingWidgetScreen::WaitingWidgetScreen(QWidget *parent) :
    QFrame{parent}
{

}

WaitingWidgetScreen::~WaitingWidgetScreen()
{

}

void WaitingWidgetScreen::paintEvent(QPaintEvent *e)
{
    QFrame::paintEvent(e);
}


WaitingWidget::WaitingWidget(QWidget *parent) :
    QDialog{parent},
    ui(new Ui::WaitingWidget)
{
    ui->setupUi(this);
//    timer = new QTimer();
}

WaitingWidget::~WaitingWidget()
{
    delete ui;
}

void WaitingWidget::update()
{

}

void WaitingWidget::Done()
{

}
