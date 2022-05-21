#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QDialog>
#include <QTimer>
#include <QTime>


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

#endif // WAITWIDGET_H
