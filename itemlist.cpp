#include "itemlist.h"
#include "itemlistwidget.h"
#include "itemeditor.h"

int available_ench_l;
EnchPlus *available_ench = new EnchPlus[INIT_LENGTH];

ItemList::ItemList(QWidget *parent)
    : QListWidget{parent}
{
    this->setViewMode(QListView::ListMode);
    this->setResizeMode(QListView::Adjust);

    connect(this, &ItemList::itemClicked, this, &ItemList::itemClickedOperation);
    connect(this, &ItemList::itemDoubleClicked, this, &ItemList::itemDoubleClickedOperation);
}


bool ItemList::setWeapon(QString name)
{
    int p = Basic::searchWeapon(name);
    if(p < 0)
    {
        qDebug() << "[EnchList::setWeapon] Error: can not find this weapon.";
        return 0;
    }
    current_weapon_index = p;

    refresh();
    return 1;
}

QString ItemList::Weapon()
{
    return Basic::weapon[current_weapon_index].name;
}

int ItemList::enchCount()
{
    return count();
}

void ItemList::getAvailable()
{
    int ench_count = 0;
    for(int i = 0; i < Basic::ench_table_l; i++)
    {
        if(Basic::ench_table[i].suitable[current_weapon_index]
            && (Basic::ench_table[i].edition == Basic::edition || Basic::ench_table[i].edition == 2))
        {
            ench_count++;
        }
    }

    EnchPlus *ench = new EnchPlus[ench_count];
    available_ench_l = 0;
    for(int i = 0, j = 0; i < Basic::ench_table_l; i++)
    {
        if(Basic::ench_table[i].suitable[current_weapon_index]
            && (Basic::ench_table[i].edition == Basic::edition || Basic::ench_table[i].edition == 2))
        {
            int p = Basic::searchEnch(Basic::origin_ench, Basic::origin_ench_l, Basic::ench_table[i].name);
            if(p != -1 && Basic::origin_ench[p].lvl == Basic::ench_table[i].mlvl)
                continue;
            int q = -1;
            for(int k = 0; k < Basic::ench_table[i].repulsion->count(); k++)
            {
                q = Basic::searchEnch(Basic::origin_ench, Basic::origin_ench_l, Basic::ench_table[i].repulsion[k]);
                if(q != -1)
                    break;
            }
            if(q == -1)
            {
                ench[j].e.name = Basic::ench_table[i].name;
                ench[j].e.lvl = Basic::origin_ench[p].lvl;
                ench[j].mlvl = Basic::ench_table[i].mlvl;
                available_ench_l++;
                j++;
            }
        }
    }
    qDebug() << "available_ench_l" << available_ench_l;

    delete [] available_ench;
    available_ench = new EnchPlus[available_ench_l];
    for(int i = 0; i < available_ench_l; i++)
    {
        available_ench[i] = ench[i];
    }
    delete [] ench;
}

void ItemList::refresh()
{
    clear();
    getAvailable();

    if(Basic::mode[1] == 1)
    {
        QListWidgetItem *item = new QListWidgetItem(this,0);

        this_item = {"", {}, 0, 0};
        ItemListWidget *w = new ItemListWidget(this);
        w->setItem(this_item);
        w->show();

//        item->setSizeHint(QSize(200,50));
        w->setLayoutMode(QLayout::SetFixedSize);
        setItemWidget(item,w);
        item->setSizeHint(w->size());
        w->setLayoutMode(QLayout::SetDefaultConstraint);

        return;
    }

    for(int i = 0; i < available_ench_l; i++)
    {
        ItemListWidget *w = new ItemListWidget(this);
        w->setEnch(available_ench[i]);
        w->show();
        QListWidgetItem *item = new QListWidgetItem(this,0);
        item->setSizeHint(QSize(200,50));
        setItemWidget(item,w);
    }

    for(int i = 0; i < count(); i++)
    {
        ItemListWidget *w = (ItemListWidget*)itemWidget(item(i));
        connect(w, &ItemListWidget::stateChanged, this, [=](){
            uploadList();
            checkRepulsion();
        });
    }
}

void ItemList::uploadList()
{
    Ench *e = new Ench[count()];

    int n = 0;
    for(int i = 0; i < count(); i++)
    {
        ItemListWidget *w = (ItemListWidget*)itemWidget(item(i));
        if(w->isChecked())
        {
            e[n] = w->ench();
            n++;
        }
    }

    delete [] Basic::needed_ench;
    Basic::needed_ench_l = n;
    Basic::needed_ench = new Ench[n];
    for(int i = 0; i < n; i++)
    {
        Basic::needed_ench[i] = e[i];
    }
    delete [] e;

    for(int i = 0; i < Basic::needed_ench_l; i++)
    {
        qDebug() << "Basic::needed_ench:" << Basic::needed_ench[i].name << Basic::needed_ench[i].lvl;
    }
}

void ItemList::checkRepulsion()
{
    for(int i = 0; i < count(); i++)
    {
        ItemListWidget *w = (ItemListWidget*)itemWidget(item(i));
        w->checkRepulsion();
    }
}

void ItemList::itemClickedOperation()
{
    if(Basic::mode[1] == 0)
    {
        ItemListWidget *w = (ItemListWidget*)itemWidget(item(currentRow()));
        if(w->isEnabled())
            w->setChecked(!w->isChecked());
    }
}

void ItemList::itemDoubleClickedOperation()
{
    if(Basic::mode[1] == 1)
    {
        ItemEditor *w = new ItemEditor(this);
        w->setModal(true);
        if(w->exec() == ItemEditor::Accepted)
        {
            this_item = w->item();
        }
    }
}

void ItemList::resizeEvent(QResizeEvent *event)
{
    QAbstractItemView::resizeEvent(event);
    if(Basic::mode[0] == 1)
    {
        emit resized();
    }
}

