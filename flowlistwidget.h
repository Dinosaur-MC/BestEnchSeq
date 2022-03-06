#ifndef FLOWLISTWIDGET_H
#define FLOWLISTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class FlowListWidget; }
QT_END_NAMESPACE

class FlowListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FlowListWidget(QWidget *parent = nullptr);
    ~FlowListWidget();

private:
    Ui::FlowListWidget *ui;

signals:

};

#endif // FLOWLISTWIDGET_H
