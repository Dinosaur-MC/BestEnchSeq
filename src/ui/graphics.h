#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>
#include <QDialog>
#include <QBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

#include <QGroupBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>

#include "core/core.h"
#include "core/algmanager.h"

namespace Ui
{
    class Graphics;
}

class Graphics : public QWidget
{
    Q_OBJECT
public:
    explicit Graphics(QWidget *parent = nullptr);
    ~Graphics();

    int run();

private:
    void setupPatterns();

    void setupTabCalc();
    void setupTabTable();
    void setupTabTool();
    void setupTabConf();

    void updateCurrentGroup(QString name);

    bool activate(int index);
    bool inactivate(int index);
    void switchTab(int n);

    int current_widget;
    int stored_pos[4];

    Ui::Graphics *ui;
    QList<QList<QWidget *>> ui_item_set;

    QList<DataTableInfo> table_info_list;
    QList<AlgorithmInfo> algorithm_list;
    Group all_enchantments;

    MCE selected_edition;
    DataTableInfo selected_table;
    Group current_group;
    ICM selected_itemconfig;
    EnchList original_enchantments;
    EnchList required_enchantments;
    ItemPool item_pool;
    QString selected_algorithm;
    ALGCFG selected_algconfig;
    FlowStack output_flows;

signals:
    void currentTabChanged();

    void tableListChanged();
    void tableContentChanged(int n);
    void settingsChanged();

    void currentEditionChanged(MCE mce);
    void currentTableChanged(int n);
    void currentGroupChanged(QString group_name);
    void currentItemConfgChanged(ICM icm);

    void claculateStarted();
    void claculatePaused();
    void claculateStopped();
    void claculateFinished();

    void originalEnchListChanged(const EnchList &enchs);
    void requriedEnchListChanged(const EnchList &enchs);
    void itemPoolChanged(const ItemPool &ip);
    void currentAlgorithmChanged(QString alg_name);
    void algorithmConfigChanged(ALGCFG algcfg);
};

// EnchListItemWidget & EnchListWidget

class EnchListItemWidget : public QWidget
{
    Q_OBJECT
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
            emit valueChanged(num.toInt());
        });
    }

    void setEnch(QString name, int min_lvl, int max_lvl)
    {
        label.setText(name);
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

protected:
    void mouseDoubleClickEvent(QMouseEvent *e) override {}

private:
    QHBoxLayout layout;
    QCheckBox chkb;
    QLabel label;
    QComboBox cbb;

signals:
    void stateChanged(bool checked);
    void valueChanged(int value);
};

class EnchListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit EnchListWidget(QWidget *parent = nullptr) : QListWidget{parent} {
        connect(this, &EnchListWidget::itemStateChanged, this, &EnchListWidget::checkConfliction);
        connect(this, &EnchListWidget::itemStateChanged, this, &EnchListWidget::listDataChanged);
        connect(this, &EnchListWidget::itemValueChanged, this, &EnchListWidget::listDataChanged);
        connect(this, &EnchListWidget::itemChanged, this, &EnchListWidget::listDataChanged);
    }

    void setEnchData(const EnchDataList &enchs, const QList<int> &min_lvls = QList<int>())
    {
        ench_data = enchs;

        this->blockSignals(true);
        this->clear();
        for (int i = 0; i < enchs.size(); i++)
        {
            EnchListItemWidget *item_widget = new EnchListItemWidget;
            item_widget->setEnch(enchs.at(i).name, min_lvls.isEmpty() ? -1 : min_lvls.at(i), enchs.at(i).max_lvl);
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(item_widget->sizeHint());
            this->addItem(item);
            this->setItemWidget(item, item_widget);

            connect(this, &EnchListWidget::itemClicked, item_widget, [=](QListWidgetItem *item){
                if (this->indexFromItem(item).row() == i && item_widget->isEnabled())
                    item_widget->setChecked(!item_widget->isChecked());
            });
            connect(item_widget, &EnchListItemWidget::stateChanged, this, [=](bool checked){
                emit itemStateChanged(i, checked);
            });
            connect(item_widget, &EnchListItemWidget::valueChanged, this, [=](int value){
                emit itemValueChanged(i, value);
            });
        }
        this->blockSignals(false);
        emit listDataChanged();
    }

    EnchList getEnchList() const
    {
        EnchList list;
        for (int i = 0; i < this->count(); i++)
        {
            EnchListItemWidget *w = (EnchListItemWidget *)this->itemWidget(this->item(i));
            if (w->isChecked())
                list.append(w->ench());
        }
        return list;
    }

private:
    void checkConfliction(int index, bool checked)
    {
        this->blockSignals(true);
        if (checked)
        {
            for (const auto &conf: ench_data.at(index).conflictions)
            {
                for (int i = 0; i < this->count(); i++)
                {
                    EnchListItemWidget *w_ = (EnchListItemWidget *)this->itemWidget(this->item(i));
                    if (w_->ench().name == conf)
                    {
                        w_->setEnabled(false);
                        break;
                    }
                }
            }
        }
        else
        {
            for (const auto &conf: ench_data.at(index).conflictions)
            {
                for (int i = 0; i < this->count(); i++)
                {
                    EnchListItemWidget *w_ = (EnchListItemWidget *)this->itemWidget(this->item(i));
                    if (w_->ench().name == conf)
                    {
                        for (const auto &cconf: ench_data.at(i).conflictions)
                        {
                            bool enabled = true;
                            for (int j = 0; j < this->count(); j++)
                            {
                                EnchListItemWidget *w__ = (EnchListItemWidget *)this->itemWidget(this->item(i));
                                if (w__->isChecked() && w__->ench().name == cconf)
                                {
                                    enabled = false;
                                    break;
                                }
                            }
                            if (enabled)
                                w_->setEnabled(true);
                        }
                    }
                }
            }
        }
        this->blockSignals(false);
    }

    EnchDataList ench_data;

signals:
    void itemStateChanged(int index, bool checked);
    void itemValueChanged(int index, int lvl);
    void listDataChanged();
};

#endif // GRAPHICS_H
