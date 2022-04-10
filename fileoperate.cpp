#include "fileoperate.h"
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>

FileOperate::FileOperate(QObject *parent)
    : QObject{parent}
{

}


void FileOperate::saveConfig()  //保存配置
{
    QFile file(FILE_CONFIG);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Error: cannot open" << FILE_CONFIG;
        return;
    }

    QString data = QString("config_version= ") + QString::number(FILEVERSION) + \
            "\ndefault_edition= " + QString::number((int)DM->config.default_edition) + \
            "\ndefault_algorithm= " + QString::number((int)DM->config.default_algorithm) +
            "\nexport_path= " + DM->config.export_path + \
            "\nconstant= " + QString::number(DM->config.constant) + \
            "\nenable_custom_weapon= " + QString::number(DM->config.enableCustomWe) + \
            "\nenable_custom_enchantment= " + QString::number(DM->config.enableCustomEn) + \
            "\nauto_check_update= " + QString::number(DM->config.autoCheckUpdate) + \
            "\n";

    file.write(data.toUtf8().data());
    file.close();
}

void FileOperate::saveWeapon()  //保存武器（用于快速自定义武器功能）
{

}

void FileOperate::saveEnchantmentTable()    //保存魔咒（用于快速自定义魔咒功能）
{

}

void FileOperate::loadConfig()  //加载配置
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
            QString str = QString("config_version= ") + QString::number(FILEVERSION) + '\n' + \
                    "default_edition= 0 \n" + \
                    "default_algorithm= 0 \n" + \
                    "export_path= \n" + \
                    "constant= 0 \n" + \
                    "enable_custom_weapon= 0 \n" + \
                    "enable_custom_enchantment= 0 \n" + \
                    "auto_check_update= 1 \n";
            file.write(str.toUtf8().data());
        }
        DM->isFirstLaunch = true;
        qDebug() << "File" << FILE_CONFIG << "created successfully.";
    }
    QStringList data = QString(file.readAll()).trimmed().split('\n');   //读取文件所有内容，优化头尾后按'\n'切片
    file.close();

    for(int i = 0; i < data.count(); i++)
    {
        data[i] = data[i].trimmed();
    }

    int exist = 0;
    for(int i = 0; i < data.count(); i++)
    {
        QStringList data2 = data[i].split('=');
        if(data2.count() == 2)
        {
            for(int j = 0; j < 2; j++)
                data2[j] = data2[j].trimmed();

            if(data2[0] == "config_version")
            {
                DM->config.config_version = data2[1].toInt();
                exist += 1;
            }
            else if(data2[0] == "default_edition")
            {
                DM->config.default_edition = (MCE)data2[1].toInt();
                exist += 1;
            }
            else if(data2[0] == "default_algorithm")
            {
                DM->config.default_algorithm = (ALGM)data2[1].toInt();
                exist += 1;
            }
            else if(data2[0] == "export_path")
            {
                DM->config.export_path = data2[1];
                exist += 1;
            }
            else if(data2[0] == "constant")
            {
                DM->config.constant = data2[1].toInt();
                exist += 1;
            }
            else if(data2[0] == "enable_custom_weapon")
            {
                DM->config.enableCustomWe = data2[1].toInt();
                exist += 1;
            }
            else if(data2[0] == "enable_custom_enchantment")
            {
                DM->config.enableCustomEn = data2[1].toInt();
                exist += 1;
            }
            else if(data2[0] == "auto_check_update")
            {
                DM->config.autoCheckUpdate = data2[1].toInt();
                exist += 1;
            }
        }
    }

    if(DM->config.config_version < FILEVERSION)
        DM->isUpdated = true;

    if(exist < CONFIG_NUM)
        saveConfig();

    qDebug() << "Config has been loaded!";
}

void FileOperate::loadWeapon()  //加载武器
{
    qDebug() << "Loading weapons infomaton...";

    QFile file(FILE_WEAPONTABLE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "[WARNING] Cannot find" << FILE_WEAPONTABLE;
        if(!file.open(QIODevice::ReadWrite))
        {
            qDebug() << "[ERROR] cannot open" << FILE_WEAPONTABLE;
            return;
        }
        file.write(FILEHEAD);
        file.close();
        qDebug() << "File" << FILE_WEAPONTABLE << "created successfully.";
        qDebug() << "[WARNING] The weapons infomaton is blank! Now is using internal data!";
        return;
    }
    //Read file data
    QStringList data_ = QString(file.readAll()).trimmed().replace("\"", "").split('\n');
    file.close();

    if(data_[0].isEmpty())
    {
        qDebug() << "[WARNING] The weapons infomaton is blank! Now is using internal data!";
        return;
    }

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
    DM->weapon_l = data.count();
    delete [] DM->weapon;
    DM->weapon = new Weapon[DM->weapon_l];
    qDebug() << "DM->weapon_l" << DM->weapon_l << data.count();

    for(int i = 0; i < DM->weapon_l && i < data.count(); i++)
    {
        QStringList data2 = data[i].trimmed().split(',');
        if(data2[0].isEmpty())
            continue;
        DM->weapon[i].name = data2[0].trimmed().replace("\\n", "\n");
        if(data2.count() <= 1)
            DM->weapon[i].icon = QIcon(":/icon/res/pack.png");
        else
            DM->weapon[i].icon = QIcon("./" + data2[1].trimmed());
    }
    qDebug() << "Weapons infomaton has been loaded!";
}

void FileOperate::loadEnchantmentTable()    //加载魔咒
{
    qDebug() << "Loading enchantments infomaton...";

    QFile file(FILE_ENCHTABLE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "[WARNING] Cannot find" << FILE_ENCHTABLE;
        if(!file.open(QIODevice::ReadWrite))
        {
            qDebug() << "[ERROR] cannot open" << FILE_ENCHTABLE;
            return;
        }
        file.write(FILEHEAD);
        file.close();
        qDebug() << "File" << FILE_ENCHTABLE << "created successfully.";
        qDebug() << "[WARNING] The enchantments infomaton is blank! Now is using internal data!";
        return;
    }
    //Read file data
    QStringList data_ = QString(file.readAll()).trimmed().split('\n');
    file.close();

    if(data_[0].isEmpty())
    {
        qDebug() << "[WARNING] The enchantments infomaton is blank! Now is using internal data!";
        return;
    }

    //Delete explanatory notes
    QStringList data;
    for(int i = 0; i < data_.count(); i++)
    {
        if(*data_[i].begin() != TEXT_NOTE_SYMBOL)
            data << data_[i];
    }


    //Store data
    DM->ench_table_l = data.count();
    delete [] DM->ench_table;
    DM->ench_table = new EnchTable[DM->ench_table_l];

    for(int i = 0; i < DM->ench_table_l && i < data.count(); i++)
    {
        QStringList data2 = data[i].trimmed().split(',');
        if(data2[0].isEmpty())
            continue;
        DM->ench_table[i].name = data2[0].trimmed();
        DM->ench_table[i].emlvl = data2[1].trimmed().toInt();
        DM->ench_table[i].mlvl = data2[2].trimmed().toInt();
        DM->ench_table[i].multiplier[0] = data2[3].trimmed().toInt();
        DM->ench_table[i].multiplier[1] = data2[4].trimmed().toInt();
        DM->ench_table[i].edition = (MCE)data2[5].trimmed().toInt();

        QStringList data3 = data2[6].trimmed().split('|');
        for(int j = 0; j < INIT_LENGTH && j < data3.count(); j++)
        {
            DM->ench_table[i].repulsion[j] = data3[j].trimmed();
        }

        for(int j = 0; j < INIT_LENGTH && j < data2.count() - 7; j++)
        {
            DM->ench_table[i].suitable[j] = data2[j+7].trimmed().toInt();
        }
    }
    qDebug() << "Enchantments infomaton has been loaded!";
}

void FileOperate::saveExport(QString dir_str)  //保存输出结果
{
    QDir dir;
    if(!dir.exists(dir_str))
        dir_str = "./exports/";

    if(!dir.exists(dir_str))
    {
          bool res = dir.mkpath(dir_str);
          if(!res)
              return;
    }

    if(DM->flow_list_l > 0)
    {
        QString name = dir_str + "output_" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmsszzz") + ".txt";
        QFile file(name);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Error: cannot output the file!";
            return;
        }
        QString head = QString("步数 Step: ") + QString::number(DM->flow_list_l) + \
                "  总等级花费 SumLevelCost: " + QString::number(DM->sumLevelCost) + \
                "  总经验花费 SumPointCost: " + QString::number(DM->sumPointCost) + \
                "\n";
        file.write(head.toUtf8().data());
        for(int i = 0; i < DM->flow_list_l; i++)
        {
            QString temp = "\n(" + QString::number(i+1) + ")  花费Cost: " + QString::number(DM->flow_list[i].cost) + "\n";

            if(DM->flow_list[i].a.name == ID_ECB)
                temp += QString("A. ") + QString(ID_ECB).simplified() + "\n";
            else
                temp += QString("A. ") + DM->OriginItem->name.simplified() + "\n";
            for(int j = 0; j < INIT_LENGTH && DM->flow_list[i].a.ench[j].name != ""; j++)
            {
                temp += "---" + DM->flow_list[i].a.ench[j].name + " [" + QString::number(DM->flow_list[i].a.ench[j].lvl) + "]\n";
            }

            if(DM->flow_list[i].b.name == ID_ECB)
                temp += QString("B. ") + QString(ID_ECB).simplified() + "\n";
            else
                temp += QString("B. ") + DM->OriginItem->name.simplified() + "\n";
            for(int j = 0; j < INIT_LENGTH && DM->flow_list[i].b.ench[j].name != ""; j++)
            {
                temp += "---" + DM->flow_list[i].b.ench[j].name + " [" + QString::number(DM->flow_list[i].b.ench[j].lvl) + "]\n";
            }

            file.write(temp.toUtf8().data());
        }
        file.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(name));
    }
}

