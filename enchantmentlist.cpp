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
    addItem("...添加附魔属性（Add Enchantment）...");

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
                }
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

        if(editor.exec() == QDialog::Accepted)
        {
            Existed[p] = editor.getSelectedEnch();
            currentItem()->setText(Existed[p].name + " - " + QString::number(Existed[p].lvl));
            if(p == count() - 1)
                addItem("...添加附魔属性（Add Enchantment）...");
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
    addItem("...添加附魔属性（Add Enchantment）...");
}
