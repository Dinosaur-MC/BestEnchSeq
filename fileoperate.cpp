#include "fileoperate.h"
#include "basic.h"
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>

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
            "\nauto_check_update= " + QString::number(Basic::config.autoCheckUpdate) + \
            "\nenable_reszie_window= " + QString::number(Basic::config.enableReszieWindow) + \
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
            QString str = QString("default_edition= 0 \n") + \
                    "default_algorithm= 0" + \
                    "export_path= \n" + \
                    "constant= 0 \n" + \
                    "enable_custom_weapon= 0 \n" + \
                    "enable_custom_enchantment= 0 \n" + \
                    "auto_check_update= 0 \n" + \
                    "enable_reszie_window=0 \n";
            file.write(str.toUtf8().data());
        }
        qDebug() << "File" << FILE_CONFIG << "created successfully.";
    }
    QStringList data = QString(file.readAll()).split('\n');
    file.close();

    for(int i = 0; i < data.count(); i++)
    {
        data[i] = data[i].trimmed();
    }

    bool exist[CONFIG_NUM] = {};
    for(int i = 0; i < data.count(); i++)
    {
        QStringList data2 = data[i].split('=');
        if(data2.count() == 2)
        {
            for(int j = 0; j < 2; j++)
                data2[j] = data2[j].trimmed();

            if(data2[0] == "default_edition")
            {
                Basic::config.default_edition = data2[1].toInt();
                exist[0] = 1;
            }
            else if(data2[0] == "default_algorithm")
            {
                Basic::config.default_algorithm = data2[1].toInt();
                exist[1] = 1;
            }
            else if(data2[0] == "export_path")
            {
                Basic::config.export_path = data2[1];
                exist[2] = 1;
            }
            else if(data2[0] == "constant")
            {
                Basic::config.constant = data2[1].toInt();
                exist[3] = 1;
            }
            else if(data2[0] == "enable_custom_weapon")
            {
                Basic::config.enableCustomWe = data2[1].toInt();
                exist[4] = 1;
            }
            else if(data2[0] == "enable_custom_enchantment")
            {
                Basic::config.enableCustomEn = data2[1].toInt();
                exist[5] = 1;
            }
            else if(data2[0] == "auto_check_update")
            {
                Basic::config.autoCheckUpdate = data2[1].toInt();
                exist[6] = 1;
            }
            else if(data2[0] == "enable_reszie_window")
            {
                Basic::config.enableReszieWindow = data2[1].toInt();
                exist[7] = 1;
            }
        }
    }

    for(int i = 0; i < CONFIG_NUM; i++)
    {
        if(!exist[i])
        {
            saveConfig();
            break;
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
    //Read file data
    QStringList data_ = QString(file.readAll()).trimmed().replace("\"", "").split('\n');
    file.close();

    //Delete explanatory notes
    QStringList data;
    for(int i = 0; i < data_.count(); i++)
    {
        if(*data_[i].begin() != TEXT_NOTE_SYMBOL)
            data << data_[i];
    }

    if(data.count() == 0)
        return;

    //Store data
    Basic::weapon_l = data.count();
    delete [] Basic::weapon;
    Basic::weapon = new Weapon[Basic::weapon_l];
    qDebug() << "Basic::weapon_l" << Basic::weapon_l << data.count();

    for(int i = 0; i < Basic::weapon_l && i < data.count(); i++)
    {
        QStringList data2 = data[i].trimmed().split(',');
        if(data2[0].isEmpty())
            continue;
        Basic::weapon[i].name = data2[0].trimmed().replace("\\n", "\n");
        if(data2.count() <= 1)
            Basic::weapon[i].icon = QIcon(":/icon/res/pack.png");
        else
            Basic::weapon[i].icon = QIcon("./" + data2[1].trimmed());
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
    //Read file data
    QStringList data_ = QString(file.readAll()).trimmed().split('\n');
    file.close();

    //Delete explanatory notes
    QStringList data;
    for(int i = 0; i < data_.count(); i++)
    {
        if(*data_[i].begin() != TEXT_NOTE_SYMBOL)
            data << data_[i];
    }

    if(data.count() == 0)
        return;

    //Store data
    Basic::ench_table_l = data.count();
    delete [] Basic::ench_table;
    Basic::ench_table = new EnchTable[Basic::ench_table_l];

    for(int i = 0; i < Basic::ench_table_l && i < data.count(); i++)
    {
        QStringList data2 = data[i].trimmed().split(',');
        if(data2[0].isEmpty())
            continue;
        Basic::ench_table[i].name = data2[0].trimmed();
        Basic::ench_table[i].mlvl = data2[1].trimmed().toInt();
        Basic::ench_table[i].multiplier[0] = data2[2].trimmed().toInt();
        Basic::ench_table[i].multiplier[1] = data2[3].trimmed().toInt();
        Basic::ench_table[i].edition = data2[4].trimmed().toInt();

        QStringList data3 = data2[5].trimmed().split('|');
        for(int j = 0; j < INIT_LENGTH && j < data3.count(); j++)
        {
            Basic::ench_table[i].repulsion[j] = data3[j].trimmed();
        }

        for(int j = 0; j < INIT_LENGTH && j < data2.count() - 6; j++)
        {
            Basic::ench_table[i].suitable[j] = data2[j+6].trimmed().toInt();
        }
    }
    qDebug() << "Enchantments infomaton has been loaded!";
}

void FileOperate::saveExport()
{
    QString dir_str;
    if(!Basic::config.export_path.isEmpty())
        dir_str = Basic::config.export_path;
    else
        dir_str = "./exports/";

    QDir dir;
    if(!dir.exists(dir_str))
    {
          bool res = dir.mkpath(dir_str);
          if(!res)
              return;
    }

    if(Basic::flow_list_l > 0)
    {
        QString name = dir_str + "output_" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmsszzz") + ".txt";
        QFile file(name);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Error: cannot output the file!";
            return;
        }
        file.write(QString(QString("步数 Step: ") + QString::number(Basic::flow_list_l) + "  总花费等级 AllCost: " + QString::number(Basic::sumCost) + "\n").toUtf8().data());
        for(int i = 0; i < Basic::flow_list_l; i++)
        {
            QString temp = "\n(" + QString::number(i+1) + ")  花费Cost: " + QString::number(Basic::flow_list[i].cost) + "\n";

            if(Basic::flow_list[i].a.name == ID_ECB)
                temp += QString("A. ") + QString(ID_ECB).simplified() + "\n";
            else
                temp += QString("A. ") + Basic::OriginItem.name.simplified() + "\n";
            for(int j = 0; j < INIT_LENGTH && Basic::flow_list[i].a.ench[j].name != ""; j++)
            {
                temp += "---" + Basic::flow_list[i].a.ench[j].name + " [" + QString::number(Basic::flow_list[i].a.ench[j].lvl) + "]\n";
            }

            if(Basic::flow_list[i].b.name == ID_ECB)
                temp += QString("B. ") + QString(ID_ECB).simplified() + "\n";
            else
                temp += QString("B. ") + Basic::OriginItem.name.simplified() + "\n";
            for(int j = 0; j < INIT_LENGTH && Basic::flow_list[i].b.ench[j].name != ""; j++)
            {
                temp += "---" + Basic::flow_list[i].b.ench[j].name + " [" + QString::number(Basic::flow_list[i].b.ench[j].lvl) + "]\n";
            }

            file.write(temp.toUtf8().data());
        }
        file.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(name));
    }
}

