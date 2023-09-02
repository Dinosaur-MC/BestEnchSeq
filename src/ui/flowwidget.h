#ifndef FLOWWIDGET_H
#define FLOWWIDGET_H

#include <QListWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include "core/core.h"

class FlowWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit FlowWidget(QWidget *parent = nullptr);

    void setFlow(const Flow &f);

private:
    Flow flow;

signals:

};

class FlowWidgetItemWidget : public QWidget
{
public:
    explicit FlowWidgetItemWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QWidget{parent, f}
    {
        label_cost.setStyleSheet("boder-radius:20; boder:3px; boder-color:green");
        label_plus.setText("+");
        label_equal.setText("=");

        this->setLayout(&layout);
        layout.addWidget(&btn_a);
        layout.addWidget(&btn_b);
        layout.addWidget(&btn_c);
        layout.addWidget(&label_a);
        layout.addWidget(&label_b);
        layout.addWidget(&label_c);
        layout.addWidget(&label_cost);
        layout.addWidget(&label_plus);
        layout.addWidget(&label_equal);
    }

    void setData(const Step &s)
    {

    }

private:
    Step step;

    QHBoxLayout layout;
    QToolButton btn_a;
    QToolButton btn_b;
    QToolButton btn_c;
    QLabel label_a;
    QLabel label_b;
    QLabel label_c;
    QLabel label_cost;
    QLabel label_plus;
    QLabel label_equal;

signals:
};

#endif // FLOWWIDGET_H
