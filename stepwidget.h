#ifndef STEPWIDGET_H
#define STEPWIDGET_H

#include <QWidget>
#include <QLayout>

namespace Ui {
class StepWidget;
}

class StepWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StepWidget(QWidget *parent = nullptr);
    ~StepWidget();

    void setIconA(QIcon icon);
    void setIconB(QIcon icon);
    void setTextA(QString str);
    void setTextB(QString str);
    void setCost(int cost);

    void setLayoutMode(QLayout::SizeConstraint a);
    void AutoWidth(int w);

private:
    Ui::StepWidget *ui;
};

#endif // STEPWIDGET_H
