#include "enchlist.h"
#include <QHBoxLayout>

EnchList::EnchList(QWidget *parent)
    : QListWidget{parent}
{
    qDebug() << "[EnchList]";
    this->setViewMode(QListView::ListMode);
    this->setResizeMode(QListView::Adjust);

    plate_l = -1;
    plate = nullptr;
    output = nullptr;
    resize = nullptr;
    current_weapon_index = -1;
    available_ench = new EnchPlus[INIT_LENGTH];
    changing = 0;

    connect(this, &EnchList::enchChanged, this, [=](){
        changing++;
    });
}

void EnchList::setOutputAddress(Ench* p, Ench* (*func)(int))
{
    output = p;
    resize = func;
}

void EnchList::setPlate(Ench *p, int len)
{
    plate_l = len;
    plate = p;
}

bool EnchList::setWeapon(QString name)
{
    current_weapon_index = BASE::sWeapon(name, 0);
    refresh();

    return 1;
}

QString EnchList::Weapon()
{
    return DM->weapon[current_weapon_index].name;
}

int EnchList::enchCount()
{
    int n = 0;
    for(int i = 0; i < count(); i++)
    {
        EnchListWidget *w = (EnchListWidget*)itemWidget(item(i));
        if(w->isChecked())
            n++;
    }
    return n;
}

void EnchList::refresh()
{
    clear();
    if(DM->ench_table_l < 1)
        return;

    if(current_weapon_index == -1)
    {
        delete [] available_ench;
        available_ench_l = DM->ench_table_l;
        available_ench = new EnchPlus[available_ench_l];
        for(int i = 0; i < INIT_LENGTH && i < DM->ench_table_l; i++)
        {
            available_ench[i].e.name = DM->ench_table[i].name;
            available_ench[i].e.lvl = 1;
            available_ench[i].mlvl = DM->ench_table[i].mlvl;
        }
    }
    else
        getAvailable();

    for(int i = 0; i < available_ench_l; i++)
    {
//        qDebug() << "available_ench - i n l ml" << i << available_ench[i].e.name << available_ench[i].e.lvl << available_ench[i].mlvl;
        EnchListWidget *w = new EnchListWidget(this);
        w->setEnch(available_ench[i], plate_l);
        w->show();
        QListWidgetItem *item = new QListWidgetItem(this,0);
        item->setSizeHint(QSize(w->sizeHint().width(), w->sizeHint().height()));
        this->setItemWidget(item,w);
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
            e[n] = w->getEnch();
            n++;
        }
    }

    output_l = n;
    if(resize == nullptr)
    {
        delete [] output;
        output = new Ench[output_l];
    }
    else
        output = resize(n);

    for(int i = 0; i < output_l; i++)
        output[i] = e[i];

    delete [] e;

    emit enchChanged();
    qDebug() << "uploadList - ench_list:Done";
}

void EnchList::getAvailable()
{
    int ench_count = 0;
    for(int i = 0; i < DM->ench_table_l; i++)
    {
        if(DM->ench_table[i].suitable[current_weapon_index]
            && (DM->ench_table[i].edition == DM->edition || DM->ench_table[i].edition == MCE::All))
        {
            ench_count++;
        }
    }

    EnchPlus *ench = new EnchPlus[ench_count];
    available_ench_l = 0;
    for(int i = 0, j = 0; i < DM->ench_table_l; i++)
    {
        if(DM->ench_table[i].suitable[current_weapon_index]
            && (DM->ench_table[i].edition == DM->edition || DM->ench_table[i].edition == MCE::All))
        {
            if(plate == nullptr)
            {
                ench[j].e.name = DM->ench_table[i].name;
                ench[j].e.lvl = 1;
                ench[j].mlvl = DM->ench_table[i].mlvl;
                available_ench_l++;
                j++;
            }
            else
            {
                int p = BASE::sEnch(plate, DM->ench_table[i].name, plate_l);
                if(p != -1 && plate[p].lvl == DM->ench_table[i].mlvl)
                    continue;
                int q = -1;
                for(int k = 0; k < DM->ench_table[i].repulsion->count(); k++)
                {
                    q = BASE::sEnch(plate, DM->ench_table[i].repulsion[k], plate_l);
                    if(q != -1)
                        break;
                }
                if(q == -1)
                {
                    ench[j].e.name = DM->ench_table[i].name;
                    if(p == -1)
                        ench[j].e.lvl = 1;
                    else
                        ench[j].e.lvl = plate[p].lvl+1;
                    ench[j].mlvl = DM->ench_table[i].mlvl;
                    available_ench_l++;
                    j++;
                }
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

void EnchList::checkRepulsion()
{
    for(int i = 0; i < count(); i++)
    {
        EnchListWidget *w = (EnchListWidget*)itemWidget(item(i));
        w->setTestList(output, output_l);
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

