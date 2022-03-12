#include "enchlist.h"
#include "enchlistwidget.h"
#include "basic.h"
#include <QHBoxLayout>

EnchList::EnchList(QWidget *parent)
    : QListWidget{parent}
{
    this->setViewMode(QListView::ListMode);
    this->setResizeMode(QListView::Adjust);
    changing = 0;
    connect(this, &EnchList::enchChanged, this, [=](){
        changing++;
    });
}


bool EnchList::setWeapon(QString name)
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

QString EnchList::Weapon()
{
    return Basic::weapon[current_weapon_index].name;
}

int EnchList::enchCount()
{
    return count();
}

void EnchList::refresh()
{
    clear();
    for(int i = 0; i < Basic::ench_table_l; i++)
    {
        if(Basic::ench_table[i].suitable[current_weapon_index] && (Basic::ench_table[i].edition == Basic::edition || Basic::ench_table[i].edition == 2))
        {
            EnchListWidget *w = new EnchListWidget(this);
            w->setEnch(Basic::ench_table[i]);
            w->show();
            QListWidgetItem *item = new QListWidgetItem(this,0);
            item->setSizeHint(QSize(200,50));
            this->setItemWidget(item,w);
        }
    }

    for(int i = 0; i < count(); i++)
    {
        EnchListWidget *w = (EnchListWidget*)itemWidget(item(i));
        connect(this, &EnchList::itemClicked, w, [=](){
            if(this->currentRow() == i && w->isEnabled())
                w->setChecked(!w->isChecked());
        });
        connect(w, &EnchListWidget::stateChanged, this, [=](){
            uploadList();
            checkRepulsion();
        });
        connect(w, &EnchListWidget::valueChanged, this, [=](){
            uploadList();
        });
    }

    uploadList();
}

void EnchList::uploadList()
{
    Ench *e = new Ench[count()];

    int n = 0;
    for(int i = 0; i < count(); i++)
    {
        EnchListWidget *w = (EnchListWidget*)itemWidget(item(i));
        if(w->isChecked())
        {
            e[n] = w->ench();
            n++;
        }
    }

    delete [] Basic::origin_ench;
    Basic::origin_ench_l = n;
    Basic::origin_ench = new Ench[n];
    for(int i = 0; i < n; i++)
    {
        Basic::origin_ench[i] = e[i];
    }

    delete [] e;
    emit enchChanged();
    qDebug() << "uploadList - origin_ench: Done";
}

void EnchList::checkRepulsion()
{
    for(int i = 0; i < count(); i++)
    {
        EnchListWidget *w = (EnchListWidget*)itemWidget(item(i));
        w->checkRepulsion();
    }
}

bool EnchList::isChanged()
{
    if(changing)
    {
        changing = 0;
        return true;
    }
    else
        return false;
}

