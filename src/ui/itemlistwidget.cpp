#include "itemlistwidget.h"

ItemListWidget::ItemListWidget(MCE mce, ItemEditor *ie, QWidget *parent)
    : QListWidget{parent}, edition(mce), item_editor(ie)
{
    if (item_editor == nullptr)
        this->QListWidget::addItem(tr("(Read only item list)"));
    else
        this->QListWidget::addItem(tr("(Click to add item)"));

    connect(this, &ItemListWidget::itemChanged, this, &ItemListWidget::listDataChanged);
    connect(this, &ItemListWidget::itemClicked, this, [=](QListWidgetItem *item){
        if (this->indexFromItem(item).row() == this->count() - 1 && item_editor != nullptr)
        {
            item_editor->refresh();
            if (item_editor->exec() == ItemEditor::Accepted)
            {
                ItemPool ip = item_editor->getItem();
                for (const Item &it: ip)
                    this->addItem(it);
            }
        }
    });
}

void ItemListWidget::clear()
{
    this->QListWidget::clear();
    if (item_editor == nullptr)
        this->QListWidget::addItem(tr("(Read only item list)"));
    else
        this->QListWidget::addItem(tr("(Click to add item)"));
}

void ItemListWidget::addItem(const Item &item)
{
    ItemListItemWidget *item_widget = new ItemListItemWidget;
    if (item_editor == nullptr)
    {
        item_widget->btn_edit.hide();
        item_widget->btn_copy.hide();
        item_widget->btn_delete.hide();
    }

    QListWidgetItem *it = new QListWidgetItem;
    this->insertItem(this->count() - 1, it);
    this->setItemWidget(it, item_widget);

    if (item_editor != nullptr)
    {
        connect(item_widget, &ItemListItemWidget::dataChanged, [=](){
            it->setSizeHint(item_widget->sizeHint());
        });
        connect(item_widget, &ItemListItemWidget::editSignal, this, [=](){
            item_editor->setItem(item);
            if (item_editor->exec() == ItemEditor::Accepted)
            {
                ItemListItemWidget *w = (ItemListItemWidget *)this->itemWidget(it);
                w->setItem(item_editor->getItem().at(0));
                emit listDataChanged();
            }
        });
        connect(item_widget, &ItemListItemWidget::copySignal, this, [=](){
            this->addItem(item_widget->getItem());
        });
        connect(item_widget, &ItemListItemWidget::deleteSignal, this, [=](){
            this->takeItem(this->indexFromItem(it).row());
        });
    }

    item_widget->setItem(item);
}

ItemPool ItemListWidget::getItemPool()
{
    item_pool.clear();

    for (int i = 0; i < this->count() - 1; i++)
    {
        ItemListItemWidget *w = (ItemListItemWidget *)this->itemWidget(this->item(i));
        Item it = w->getItem();
        for (Ench &e: it.enchs)
            e.mce = edition;
        item_pool.append(it);
    }

    return item_pool;
}
