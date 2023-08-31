#ifndef ITEMLISTWIDGET_H
#define ITEMLISTWIDGET_H

#include "core/core.h"
#include <QListWidget>
#include <QWidget>
#include "itemeditor.h"

#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>

class ItemListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ItemListWidget(MCE mce, ItemEditor *ie = nullptr, QWidget *parent = nullptr);

    void clear();
    void addItem(const Item &item);
    ItemPool getItemPool();

private:
    MCE edition;
    ItemPool item_pool;

    ItemEditor *item_editor;

signals:
    void listDataChanged();
};

class ItemListItemWidget : public QWidget
{
    Q_OBJECT
    friend ItemListWidget;
public:
    explicit ItemListItemWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QWidget{parent, f}
    {
        label_name.setAlignment(Qt::AlignTop);
        btn_edit.setIconSize(QSize(24, 24));
        btn_edit.setIcon(QIcon(":/icon/editor.svg"));
        btn_copy.setIconSize(QSize(24, 24));
        btn_copy.setIcon(QIcon(":/icon/copy.svg"));
        btn_delete.setIconSize(QSize(24, 24));
        btn_delete.setIcon(QIcon(":/icon/delete.svg"));

        this->setLayout(&layout);
        layout.addWidget(&label_name);
        layout.addWidget(&label_ench);
        layout.addWidget(&label_addition);
        layout.addWidget(&btn_edit);
        layout.addWidget(&btn_copy);
        layout.addWidget(&btn_delete);

        connect(&btn_edit, &QToolButton::clicked, this, [=](){
            emit editSignal();
        });
        connect(&btn_copy, &QToolButton::clicked, this, [=](){
            emit copySignal();
        });
        connect(&btn_delete, &QToolButton::clicked, this, [=](){
            emit deleteSignal();
        });
    }

    void setItem(Item it)
    {
        item = it;

        if (item.type == ItemType::Stuff)
        {
            label_name.setText(tr("Stuff"));
            label_ench.setText("[" + QString::number(item.durability) + "]");
            label_addition.setText("");
        }
        else
        {
            if (item.type == ItemType::Book)
                label_name.setText(tr("Enchanted Book"));
            else if (item.type == ItemType::Weapon)
                label_name.setText(tr("Weapon"));

            QString str_enchs = "";
            for (const Ench &e: item.enchs)
                str_enchs += e.name + ' ' + intToLevelText(e.lvl) + '\n';
            str_enchs.removeLast();
            label_ench.setText(str_enchs);

            label_addition.setText(tr("Penalty") + "[" + QString::number(item.penalty) + "]\t" + tr("Durability") + "[" + QString::number(item.durability) + "]");
        }

        emit dataChanged();
    }

    Item getItem() const
    {
        return item;
    }

private:
    Item item;

    QHBoxLayout layout;
    QLabel label_name;
    QLabel label_ench;
    QLabel label_addition;
    QToolButton btn_edit;
    QToolButton btn_copy;
    QToolButton btn_delete;

signals:
    void dataChanged();
    void editSignal();
    void copySignal();
    void deleteSignal();
};

#endif // ITEMLISTWIDGET_H
