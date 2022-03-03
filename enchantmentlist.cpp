#include "enchantmentlist.h"
#include "selector.h"
#include <QPushButton>

using namespace std;

int lever;

EnchantmentList::EnchantmentList(QWidget *parent)
    : QListWidget{parent}
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setViewMode(QListView::ListMode);

    QListWidgetItem *newItem = new QListWidgetItem("---添加附魔属性（Add Enchantment）---");
    newItem->setSizeHint(QSize(0,36));
    newItem->setFont(QFont(newItem->font().family(), 16));
    addItem(newItem);

    connect(this, &QListWidget::itemClicked, this, [=](){
        int p = currentRow();
        qDebug() << "Clicked" << p;

        if(p != count() - 1)
        {
            QDialog choose;
            choose.setFixedSize(250,100);
            choose.setWindowTitle("What do you want to do?");
            QPushButton btnEdit("Edit");
            btnEdit.setParent(&choose);
            btnEdit.move(35,36);
            QPushButton btnDelete("Delete");
            btnDelete.setParent(&choose);
            btnDelete.move(142,36);

            choose.setModal(true);
            lever = 1;

            connect(&btnEdit, &QPushButton::clicked, &choose, [=](){
                lever = 0;
            });
            connect(&btnEdit, &QPushButton::clicked, &choose, &QDialog::close);

            connect(&btnDelete, &QPushButton::clicked, &choose, [=](){
                takeItem(p);
                for(int i = 0; Existed[p+i].name != "" && p+i+1 < 64; i++)
                {
                    Existed[p+i].name = Existed[p+i+1].name;
                    Existed[p+i].lvl = Existed[p+i+1].lvl;
                }
                item(count()-1)->setText("---添加附魔属性（Add Enchantment）---");
                lever = 1;
            });
            connect(&btnDelete, &QPushButton::clicked, &choose, &QDialog::close);

            choose.show();
            choose.exec();

            if(lever == 1)
                return;
        }

        Selector editor;
        Ench Current;
        if(p != count() - 1)
        {
            Current.name = Existed[p].name;
            Current.lvl = Existed[p].lvl;
        }
        else
        {
            Current.name = "";
            Current.lvl = 0;
        }
        editor.setup(Current, Existed, Listmode);

        if(editor.isEmpty())
        {
            currentItem()->setText("---没有可用附魔（Empty）---");
        }
        else
        {
            if(editor.exec() == QDialog::Accepted)
            {
                Existed[p] = editor.getSelectedEnch();
                currentItem()->setText(Existed[p].name + " - " + QString::number(Existed[p].lvl));
                if(p == count() - 1)
                {
                    QListWidgetItem *newItem_ = new QListWidgetItem("---添加附魔属性（Add Enchantment）---");
                    newItem_->setSizeHint(QSize(0,36));
                    newItem_->setFont(QFont(newItem_->font().family(), 16));
                    addItem(newItem_);
                }
                item(0)->setForeground(QBrush(QColor(0,0,0)));
                if(editor.availableCount() == 1)
                    item(count()-1)->setText("---没有可用附魔（Empty）---");
            }
        }
    });
}


void EnchantmentList::refreshEnch()
{
    for(int i = 0; i < 64; i++)
    {
        Common::Existed[i] = Existed[i];
    }
}

void EnchantmentList::setMode(int a)
{
    Listmode = a;
}

Ench *EnchantmentList::getEnch()
{
    return Existed;
}

void EnchantmentList::clearEnch()
{
    clear();
    for(int i = 0; i < 64; i++)
    {
        Existed[i] = {};
    }
    QListWidgetItem *newItem_ = new QListWidgetItem("---添加附魔属性（Add Enchantment）---");
    newItem_->setSizeHint(QSize(0,36));
    newItem_->setFont(QFont(newItem_->font().family(), 16));
    addItem(newItem_);
}
