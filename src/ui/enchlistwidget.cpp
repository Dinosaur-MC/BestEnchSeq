#include "enchlistwidget.h"

EnchListWidget::EnchListWidget(QWidget *parent)
    : QListWidget{parent}
{
    connect(this, &EnchListWidget::itemStateChanged, this, &EnchListWidget::checkConfliction);
    connect(this, &EnchListWidget::itemStateChanged, this, &EnchListWidget::listDataChanged);
    connect(this, &EnchListWidget::itemValueChanged, this, &EnchListWidget::listDataChanged);
    connect(this, &EnchListWidget::itemChanged, this, &EnchListWidget::listDataChanged);
}

void EnchListWidget::setEnchData(const EnchDataList &enchs, const QList<int> &min_lvls)
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

void EnchListWidget::setChecked(int p, bool checked)
{
    EnchListItemWidget *w = (EnchListItemWidget *)this->itemWidget(this->item(p));
    w->setChecked(checked);
}

void EnchListWidget::setEnchLevel(int p, int value)
{
    EnchListItemWidget *w = (EnchListItemWidget *)this->itemWidget(this->item(p));
    w->setValue(value);
}

EnchList EnchListWidget::getEnchList() const
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

void EnchListWidget::checkConfliction(int index, bool checked)
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
