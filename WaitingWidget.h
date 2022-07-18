#ifndef WAITINGWIDGET_H
#define WAITINGWIDGET_H

#include <QObject>
#include <QDialog>
#include <QFrame>
#include <QTime>


class WaitingWidgetScreen : public QFrame
{
    Q_OBJECT

public:
    explicit WaitingWidgetScreen(QWidget *parent = nullptr);
    ~WaitingWidgetScreen();

private:

protected:
    void paintEvent(QPaintEvent *event);

};


namespace Ui {
class WaitingWidget;
}

class WaitingWidget : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingWidget(QWidget *parent = nullptr);
    ~WaitingWidget();

    void update();
    void Done();

private:
    Ui::WaitingWidget *ui;
    QTimer *timer;
    QTime st;
    QTime et;

};

#endif // WAITINGWIDGET_H
