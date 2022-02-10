#include "selector.h"
#include "ui_selector.h"

using namespace std;

int Mode = 0; //Tab index   Origin|Needed

Selector::Selector(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Selector)
{
    ui->setupUi(this);
    ui->Level->setMinimumWidth(32);
    setModal(true);

    connect(ui->Enchantment, &QComboBox::currentTextChanged, this, [=](){
        for(int i = 0; &Available[i] != end(Available) && Available[i].name != ""; i++)
        {
            if(ui->Enchantment->currentText() == Available[i].name)
            {
                ui->Level->setMaximum(Available[i].lvl);
            }

            if(Mode == 1)
            {
                for(int i = 0; &Avoid2[i] != end(Avoid2) && Avoid2[i].name != ""; i++)
                {
                    if(Avoid2[i].name == ui->Enchantment->currentText())
                    {
                        ui->Level->setMinimum(Avoid2[i].lvl + 1);
                        break;
                    }
                    else
                    {
                        ui->Level->setMinimum(1);
                    }
                }
            }
        }
    });

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=](){
        if(ui->Enchantment->currentText() != "")
            accept();
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(this, &QDialog::accepted, this, [=](){
        qDebug() << "Accepted";
    });

}

Selector::~Selector()
{
    delete ui;
}

void Selector::setup(Ench iCurrent, Ench *iExisted, int m)
{
    qDebug() << "Setup Selector";
    //Avoid List
    Mode = m;
    Current = iCurrent;
    for(int i = 0, j = 0; iExisted[i].name != "" && i < 64; j++)
    {
        if(iExisted[j].name != Current.name)
        {
            Avoid[i] = iExisted[j];
            i++;
        }
    }
    if(Mode == 1)
    {
        for(int i = 0; Common::Existed[i].name != "" && i < 64; i++)
        {
            Avoid2[i] = Common::Existed[i];
        }
    }

    //Debug
    for(int i = 0; &Avoid[i] != end(Avoid) && Avoid[i].name != "" ; i++)
    {
        qDebug() << "AvoidList:" << Avoid[i].name;
    }
    qDebug() << "Current:" << Current.name;
    for(int i = 0; &Avoid2[i] != end(Avoid2) && Avoid2[i].name != "" ; i++)
    {
        qDebug() << "AvoidList 2:" << Avoid2[i].name;
    }

    refresh();
    if(Available[0].name == "")
        return;
    show();
}

void Selector::refresh()
{
    //Available Enchantment List
    Ench Temp[64];
    if(Mode == 1)
    {
        for(int i = 0; i < 64; i++)
        {
            Temp[i] = Avoid[i];
        }

        int i = 0;
        while(Avoid[i].name != "" && &Avoid[i] != end(Avoid))
            i++;
        for(int j = 0; &Avoid[i+j] != end(Avoid) && Avoid2[j].name != ""; j++)
        {
            Avoid[i+j] = Avoid2[j];
        }
    }

    qDebug() << "Step 1" << Common::InputItem.type;
    QStringList typeName;
    typeName << "剑\nSword" << "镐\nPickaxe" << "斧\nAxe" << "铲\nShovel" << "锄\nHoe" << "头盔\nHelmet" << "胸甲\nChestplate" << "护腿\nLeggings" << "靴\nBoots" << "弓\nBow" << "弩\nCrossbow" << "三叉戟\nTrident" << "钓鱼竿\nFishing Rod";
    for(int i = 0; &typeName[i] != end(typeName); i++)
    {
        if(Common::InputItem.type == typeName[i])
        {
            qDebug() << "Type:" << typeName[i];
            for(int j = 0, k = 0; &Common::Table[k] != end(Common::Table) && j < 64; k++)
            {
                if(Common::Table[k].suitable[i])
                {
                    Available[j].name = Common::Table[k].name;
                    Available[j].lvl = Common::Table[k].max_lvl;
                    j++;
                }
            }
            break;
        }
    }

    qDebug() << "Step 2";
    for(int i = 0; &Avoid[i] != end(Avoid) && Avoid[i].name != ""; i++)
    {
        for(int j = 0; &Common::Table[j] != end(Common::Table) && Common::Table[j].name != ""; j++)
        {
            if(Avoid[i].name == Common::Table[j].name)
            {
                qDebug() << "Avoid -> Table" << Common::Table[j].name;
                for(int k = 0; k < Common::Table[j].repulsion.count(); k++)
                {
                    for(int l = 0; &Available[l] != end(Available) && Available[l].name != ""; l++)
                    {
                        if(Common::Table[j].repulsion[k] == Available[l].name)
                        {
                            qDebug() << "Removed:" << Available[l].name;
                            int m = 0;
                            for(; &Available[l+m+1] != end(Available); m++)
                            {
                                Available[l+m] = Available[l+m+1];
                            }
                            Available[l+m] = {};
                            break;
                        }
                    }
                }
                break;
            }
        }
    }

    qDebug() << "Step 3" << "Mode:" << Mode;
    if(Mode == 1)
    {
        for(int i = 0; i < 64; i++)
        {
            Avoid[i] = Temp[i];
        }

        int i = 0;
        while(Avoid[i].name != "" && &Avoid[i] != end(Avoid))
            i++;
        for(int j = 0, k = 0; &Avoid[i+j] != end(Avoid) && &Avoid2[k] != end(Avoid2) && Avoid2[k].name != ""; k++)
        {
            if(Avoid2[k].lvl == Common::Table[Common::searchTable(Avoid2[k].name)].max_lvl)
            {
                Avoid[i+j] = Avoid2[k];
                j++;
            }
        }
    }
    for(int i = 0; &Available[i] != end(Available) && Available[i].name != ""; i++)
    {
        for(int j = 0; &Avoid[j] != end(Avoid) && Avoid[j].name != ""; j++)
        {
            if(Available[i].name == Avoid[j].name)
            {
                qDebug() << "Removed:" << Available[i].name;
                int k = 0;
                for(; &Available[i+k+1] != end(Available); k++)
                {
                    Available[i+k] = Available[i+k+1];
                }
                Available[i+k] = {};
                i--;
                break;
            }
        }
    }

    qDebug() << "Step 4";
    for(int i = 0; &Available[i] != end(Available) && Available[i].name != ""; i++)
    {
        ui->Enchantment->addItem(Available[i].name);
    }
    if(Current.name != "")
    {
        ui->Enchantment->setCurrentText(Current.name);
        ui->Level->setValue(Current.lvl);
    }
    else
    {
        if(Available[0].name != "")
        {
            ui->Level->setEnabled(1);
            ui->Enchantment->setCurrentText(Available[0].name);
            ui->Level->setMaximum(Available[0].lvl);
            if(Mode == 1)
            {
                for(int i = 0; &Avoid2[i] != end(Avoid2) && Avoid2[i].name != ""; i++)
                {
                    if(Avoid2[i].name == Available[0].name)
                    {
                        ui->Level->setMinimum(Avoid2[i].lvl + 1);
                        break;
                    }
                }
            }
        }
        else
        {
            ui->Level->setMinimum(0);
            ui->Level->setMaximum(0);
            ui->Level->setEnabled(0);
        }
    }

}

Ench Selector::getSelectedEnch()
{
    Ench a;
    a.name = ui->Enchantment->currentText();
    a.lvl = ui->Level->value();
    return a;
}

bool Selector::isEmpty()
{
    if(Available[0].name == "")
        return 1;
    else
        return 0;
}

int Selector::availableCount()
{
    int count = 0;
    while(Available[count].name != "")
        count++;
    return count;
}

