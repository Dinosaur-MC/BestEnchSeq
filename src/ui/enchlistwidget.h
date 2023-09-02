#ifndef ENCHLISTWIDGET_H
#define ENCHLISTWIDGET_H

#include "core/core.h"
#include <QListWidget>
#include <QWidget>

#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>

class EnchListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit EnchListWidget(QWidget *parent = nullptr);

    void setEnchData(const EnchDataList &enchs, const QList<int> &min_lvls = QList<int>());
    void setChecked(int p,bool checked);
    void setEnchLevel(int p, int value);

    EnchList getEnchList() const;

private:
    void checkConfliction(int index, bool checked);

    EnchDataList ench_data;

signals:
    void itemStateChanged(int index, bool checked);
    void itemValueChanged(int index, int lvl);
    void listDataChanged();
};

class EnchListItemWidget : public QWidget
{
    Q_OBJECT
    friend EnchListWidget;
public:
    explicit EnchListItemWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QWidget{parent, f}
    {
        this->setLayout(&layout);
        layout.addWidget(&chkb);
        layout.addWidget(&label);
        layout.addWidget(&cbb);

        label.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        connect(&chkb, &QCheckBox::stateChanged, this, [=](int state){
            emit stateChanged(state);
        });
        connect(&cbb, &QComboBox::currentTextChanged, this, [=](const QString &num){
            if (!chkb.isChecked())
                chkb.setChecked(true);
            emit valueChanged(num.toInt());
        });
    }

    void setEnch(QString name, int min_lvl, int max_lvl)
    {
        label.setText(name);

        cbb.blockSignals(true);
        for (int i = min_lvl <= 1 ? 1 : min_lvl; i <= max_lvl; i++)
            cbb.addItem(QString::number(i));
        if (cbb.count() > 0)
        {
            if (min_lvl < 0)
                cbb.setCurrentIndex(0);
            else
                cbb.setCurrentIndex(cbb.count()-1);
        }
        else
            this->setEnabled(false);
        cbb.blockSignals(false);

        emit enchDataChanged();
    }

    Ench ench() const
    {
        return {label.text(), MCE::Null, cbb.currentText().toInt()};
    }

    void setChecked(bool checked)
    {
        chkb.setChecked(checked);
    }

    bool isChecked() const
    {
        return chkb.isChecked();
    }

    void setValue(int value)
    {
        cbb.setCurrentText(QString::number(value));
    }

    int value()
    {
        return cbb.currentText().toInt();
    }

protected:
    void mouseDoubleClickEvent(QMouseEvent *e) override {}

private:
    QHBoxLayout layout;
    QCheckBox chkb;
    QLabel label;
    QComboBox cbb;

signals:
    void enchDataChanged();
    void stateChanged(bool checked);
    void valueChanged(int value);
};

#endif // ENCHLISTWIDGET_H
