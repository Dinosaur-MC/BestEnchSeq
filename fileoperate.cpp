#include "fileoperate.h"
#include "basic.h"
#include <QFile>

FileOperate::FileOperate(QObject *parent)
    : QObject{parent}
{

}


void FileOperate::saveConfig()
{
    QFile file(FILE_CONFIG);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Error: cannot open" << FILE_CONFIG;
        return;
    }

    QString data = QString("default_edition= ") + QString::number(Basic::config.default_edition) + \
            "\ndefault_algorithm= " + QString::number(Basic::config.default_algorithm) +
            "\nexport_path= " + Basic::config.export_path + \
            "\nconstant= " + QString::number(Basic::config.constant) + \
            "\nenable_custom_weapon= " + QString::number(Basic::config.enableCustomWe) + \
            "\nenable_custom_enchantment= " + QString::number(Basic::config.enableCustomEn) + \
            "\n";

    file.write(data.toUtf8().data());
    file.close();
}

void FileOperate::saveWeapon()
{

}

void FileOperate::saveEnchantmentTable()
{

}

void FileOperate::loadConfig()
{
    qDebug() << "Loading config...";

    QFile file(FILE_CONFIG);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Warn: cannot find" << FILE_CONFIG;
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << "Error: cannot open" << FILE_CONFIG;
            return;
        }
        else
        {
            file.write(
"\
default_edition= 0 \n\
default_algorithm= 0 \n\
export_path= \n\
constant= 0 \n\
enable_custom_weapon= 0 \n\
enable_custom_enchantment= 0 \n\
"
                    );
        }
        qDebug() << "File" << FILE_CONFIG << "created successfully.";
    }
    QStringList data = QString(file.readAll()).split('\n');
    file.close();

    for(int i = 0; i < data.count(); i++)
    {
        data[i] = data[i].trimmed();
    }

    for(int i = 0; i < data.count(); i++)
    {
        QStringList data2 = data[i].split('=');
        if(data2.count() == 2)
        {
            for(int j = 0; j < 2; j++)
            {
                data2[j] = data2[j].trimmed();
            }

            if(data2[0] == "default_edition")
            {
                Basic::config.default_edition = data2[1].toInt();
            }
            else if(data2[0] == "default_algorithm")
            {
                Basic::config.default_algorithm = data2[1].toInt();
            }
            else if(data2[0] == "export_path")
            {
                Basic::config.export_path = data2[1];
            }
            else if(data2[0] == "constant")
            {
                Basic::config.constant = data2[1].toInt();
            }
            else if(data2[0] == "enable_custom_weapon")
            {
                Basic::config.enableCustomWe = data2[1].toInt();
            }
            else if(data2[0] == "enable_custom_enchantment")
            {
                Basic::config.enableCustomEn = data2[1].toInt();
            }
        }
    }

    qDebug() << "Config has been loaded!";
}

void FileOperate::loadWeapon()
{
    qDebug() << "Loading weapons infomaton...";

    QFile file(FILE_WEAPONTABLE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Warn: cannot find" << FILE_WEAPONTABLE;
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << "Error: cannot open" << FILE_WEAPONTABLE;
            return;
        }
        qDebug() << "File" << FILE_WEAPONTABLE << "created successfully.";
    }
    QStringList data = QString(file.readAll()).split('\n');
    file.close();

    Basic::weapon_l = data.count();
    delete [] Basic::weapon;
    Basic::weapon = new Weapon[Basic::weapon_l];
    qDebug() << "Basic::weapon_l" << Basic::weapon_l << data.count();

    for(int i = 0; i < Basic::weapon_l && i < data.count(); i++)
    {
        QStringList data2 = data[i].split(',');
        if(data2[0].isEmpty())
            continue;
        Basic::weapon[i].name = data2[0];
        if(data2.count() <= 1)
            Basic::weapon[i].icon = QIcon(":/icon/res/pack.png");
        else
            Basic::weapon[i].icon = QIcon("./" + data2[1]);
    }
    qDebug() << "Weapons infomaton has been loaded!";
}

void FileOperate::loadEnchantmentTable()
{
    qDebug() << "Loading enchantments infomaton...";

    QFile file(FILE_ENCHTABLE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Warn: cannot find" << FILE_ENCHTABLE;
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << "Error: cannot open" << FILE_ENCHTABLE;
            return;
        }
        qDebug() << "File" << FILE_ENCHTABLE << "created successfully.";
    }
    QStringList data = QString(file.readAll()).split('\n');
    file.close();

    if(data.count() == 0)
    {
        return;
    }
    Basic::ench_table_l = data.count();
    delete [] Basic::ench_table;
    Basic::ench_table = new EnchTable[Basic::ench_table_l];

    for(int i = 0; i < Basic::ench_table_l && i < data.count(); i++)
    {
        QStringList data2 = data[i].split(',');
        if(data2[0].isEmpty())
            continue;
        Basic::ench_table[i].name = data2[0];
        Basic::ench_table[i].mlvl = data2[1].toInt();
        Basic::ench_table[i].multiplier[0] = data2[2].toInt();
        Basic::ench_table[i].multiplier[1] = data2[3].toInt();

        QStringList data3 = data2[4].split('|');
        for(int j = 0; j < INIT_LENGTH && j < data3.count(); j++)
        {
            Basic::ench_table[i].repulsion[j] = data3[j];
        }

        for(int j = 0; j < INIT_LENGTH && j < data2.count() - 5; j++)
        {
            Basic::ench_table[i].suitable[j] = data2[j+5].toInt();
        }
    }
    qDebug() << "Enchantments infomaton has been loaded!";
}

void FileOperate::saveExport()
{

}

