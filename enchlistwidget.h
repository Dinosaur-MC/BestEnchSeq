#ifndef ENCHLISTWIDGET_H
#define ENCHLISTWIDGET_H

#include <QWidget>
#include "basic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EnchListWidget; }
QT_END_NAMESPACE

class EnchListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EnchListWidget(QWidget *parent = nullptr);
    ~EnchListWidget();

    void setEnch(EnchTable ench);
    Ench ench();
    void setChecked(bool);
    bool isChecked();
    void checkRepulsion();

private:
    Ui::EnchListWidget *ui;
    int current_row;

signals:
    void stateChanged();
    void valueChanged();
};

#endif // ENCHLISTWIDGET_H
