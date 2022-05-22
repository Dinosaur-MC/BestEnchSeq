#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include "datamanager.h"
#include <QDialog>
#include <QTimer>
#include <QTime>

#include <QFrame>


QT_BEGIN_NAMESPACE
namespace Ui {class WaitWidget;}
QT_END_NAMESPACE

class WaitWidget : public QDialog
{
    Q_OBJECT
public:
    explicit WaitWidget(QWidget *parent = nullptr);
    ~WaitWidget();

    void update();
    void Done();

private:
    Ui::WaitWidget *ui;
    QTimer *timer;
    QTime st;
    QTime et;

};

class WaitWidgetScreen : public QFrame
{
    Q_OBJECT
public:
    explicit WaitWidgetScreen(QWidget *parent = nullptr);
    ~WaitWidgetScreen();

private:

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // WAITWIDGET_H
