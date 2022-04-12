#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QDialog>


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

#endif // WAITWIDGET_H
