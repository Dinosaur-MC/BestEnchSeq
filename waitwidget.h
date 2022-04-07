#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QDialog>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {class WaitWidget;}
QT_END_NAMESPACE

class WaitWidget : public QDialog
{
    Q_OBJECT
public:
    explicit WaitWidget(QWidget *parent = nullptr);
    ~WaitWidget();

private:
    Ui::WaitWidget *ui;

};

class WaitWidgetThread : public QThread
{
    Q_OBJECT
protected:
    void run();

signals:
    void isDone();
};

#endif // WAITWIDGET_H
