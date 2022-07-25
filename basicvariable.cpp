#include "basicvariable.h"
#include "basicoperator.h"


CfgOpr::CfgOpr(Config *c){
    cfg = c;

    qDebug() << "[CfgOpr] Initialized.";
}

void CfgOpr::link(Config* c) {
    cfg = c;
}

void* CfgOpr::operator[](int i) {
    switch (i) {
    case 0:
        return &cfg->config_version;
        break;
    case 1:
        return &cfg->default_edition;
        break;
    case 2:
        return &cfg->default_itemconfig;
        break;
    case 3:
        return &cfg->default_algorithm;
        break;
    case 4:
        return &cfg->export_path;
        break;
    case 5:
        return &cfg->constant;
        break;
    case 6:
        return &cfg->auto_check_update;
        break;
    case 7:
        return &cfg->enable_custom_we;
        break;
    case 8:
        return &cfg->enable_custom_en;
        break;
    case 9:
        return &cfg->enable_reszie_window;
        break;
    case 10:
        return &cfg->deverloper_mode;
        break;
    default:
        return NULL;
        break;
    }
}


bool operator<(const Ench a, const Ench b)
{
    if(a.lvl < b.lvl)
        return true;
    else
        return false;
}

bool operator>(const Ench a, const Ench b)
{
    if(a.lvl > b.lvl)
        return true;
    else
        return false;
}


Chameleon::Chameleon(QVector<raw_EnchPlus> *reps)
{
    raw_ench_table = reps;
    isRich = false;
    lvl = NULL;

    qDebug() << "[Chameleon] Initialized.";
}

Chameleon::~Chameleon()
{
    if(isRich)
        delete lvl;
}

bool Chameleon::link(QVector<raw_EnchPlus> *reps)
{
    if(reps == NULL)
        return false;
    else
        raw_ench_table = reps;
    return true;
}

Chameleon *Chameleon::fromVEnch(QVector<Ench> es)
{
    if(es.count() < 1)
        return NULL;

    data.clear();
    if(lvl == NULL)
        lvl = new QVector<int>;
    else
    {
        delete lvl;
        lvl = new QVector<int>;
    }


    for(int i = 0; i < es.count(); i++)
    {
        data.append(es.at(i).id);
        lvl->append(es.at(i).lvl);
    }

    data.squeeze();
    lvl->squeeze();

    isRich = true;
    return this;
}

Chameleon *Chameleon::fromVEnchPlus(QVector<EnchPlus> eps)
{
    if(eps.count() < 1)
        return NULL;

    data.clear();
    for(int i = 0; i < eps.count(); i++)
        data.append(eps.at(i).id);
    data.squeeze();

    if(isRich)
    {
        delete lvl;
        lvl = NULL;
        isRich = false;
    }
    return this;
}

Chameleon *Chameleon::fromVEnchPro(QVector<EnchPro> eprs)
{
    if(eprs.count() == 0)
        return NULL;

    data.clear();
    for(int i = 0; i < eprs.count(); i++)
        data.append(eprs.at(i).id);
    data.squeeze();

    if(isRich)
    {
        delete lvl;
        lvl = NULL;
        isRich = false;
    }
    return this;
}

Chameleon *Chameleon::fromVRawEnchPlus(QVector<raw_EnchPlus> reps)
{
    if(reps.count() == 0)
        return NULL;

    data.clear();
    for(int i = 0; i < reps.count(); i++)
    {
        for(int j = 0; j < raw_ench_table->count(); j++)
        {
            if(reps.at(i).edition == raw_ench_table->at(j).edition && reps.at(i).name == raw_ench_table->at(j).name)
                data.append(j);
        }
    }
    data.squeeze();

    if(isRich)
    {
        delete lvl;
        lvl = NULL;
        isRich = false;
    }
    return this;
}


QVector<Ench> Chameleon::toVEnch()
{
    QVector<Ench> es;
    if(isRich)
    {
        for(int i = 0; i < data.count(); i++)
            es.append({data.at(i), lvl->at(i)});
    }
    else
    {
        for(int i = 0; i < data.count(); i++)
            es.append({data.at(i), 1});
    }
    return es;
}

QVector<EnchPlus> Chameleon::toVEnchPlus()
{
    QVector<EnchPlus> eps;
    for(int i = 0; i < data.count(); i++)
    {
        EnchPlus tm;
        tm.id = data.at(i);
        tm.edition = raw_ench_table->at(data.at(i)).edition;
        tm.poor_max_level = raw_ench_table->at(data.at(i)).poor_max_level;
        tm.max_level = raw_ench_table->at(data.at(i)).max_level;
        tm.multiplier[0] = raw_ench_table->at(data.at(i)).multiplier[0];
        tm.multiplier[1] = raw_ench_table->at(data.at(i)).multiplier[1];
        for(int j = 0; j < raw_ench_table->at(data.at(i)).repulsion.count(); j++)
        {
            for(int k = 0; k < raw_ench_table->count(); k++)
            {
                if(raw_ench_table->at(data.at(i)).repulsion.at(j) == raw_ench_table->at(k).name)
                    tm.repulsion.append(k);
            }
        }
        eps.append(tm);
    }
    return eps;
}

QVector<EnchPro> Chameleon::toVEnchPro()
{
    QVector<EnchPro> eprs;
    for(int i = 0; i < data.count(); i++)
    {
        EnchPro tm;
        tm.id = data.at(i);
        tm.minimum = (isRich? lvl->at(i): 1);
        tm.maximum = raw_ench_table->at(data.at(i)).max_level;
        tm.text = raw_ench_table->at(data.at(i)).name;
        eprs.append(tm);
    }
    return eprs;
}

QVector<Item> Chameleon::toVEBook()
{
    QVector<Item> its;
    QVector<Ench> es = this->toVEnch();
    for(int i = 0; i < data.count(); i++)
    {
        Item tm;
        tm.type = -1;
        tm.ench.append(es.at(i));
        tm.durability = 0;
        tm.penalty = 0;
        its.append(tm);
    }
    return its;
}

Item Chameleon::toEBook()
{
    Item it;
    it.type = -1;
    it.ench = this->toVEnch();
    it.durability = 0;
    it.penalty = 0;
    return it;
}

void Chameleon::clear()
{
    data.clear();
    lvl->clear();
    isRich = false;
}

