#include "waitingwidget.h"
#include "ui_waitingwidget.h"


WaitingWidgetScreen::WaitingWidgetScreen(QWidget *parent) :
    QFrame{parent}
{

}

WaitingWidgetScreen::~WaitingWidgetScreen()
{

}


WaitingWidget::WaitingWidget(QWidget *parent) :
    QDialog{parent},
    ui(new Ui::WaitingWidget)
{

}

WaitingWidget::~WaitingWidget()
{
    delete ui;
}
