#include "basicoperator.h"
#include <QDateTime>
#include <QFile>
#include <QUrl>
#include <QDir>

FileOperator::FileOperator(QObject *parent) :
    QObject{parent}
{
    qDebug() << "[FileOperator] Initialized.";
}

void FileOperator::saveConfig(Config config, const QVector<Option> *opt) // 保存配置
{
    qDebug() << "[FileOperator] Saving configuration...";

    QFile file(FILE_CONFIG);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error: cannot open" << FILE_CONFIG;
        return;
    }

    QString data = "";
    for(int i = 0; i < CONFIG_NUM; i++)
    {
        CfgOpr opr;
        opr.link(&config);
        if(opt->at(i).value_type == ValueType::Bool)
            data += opt->at(i).name + "= " + QString::number(*(bool*)opr[i]) + "\n";
        if(opt->at(i).value_type == ValueType::Int)
            data += opt->at(i).name + "= " + QString::number(*(int*)opr[i]) + "\n";
        else if(opt->at(i).value_type == ValueType::Float)
            data += opt->at(i).name + "= " +  QString::number(*(float*)opr[i]) + "\n";
        else if(opt->at(i).value_type == ValueType::Double)
            data += opt->at(i).name + "= " +  QString::number(*(double*)opr[i]) + "\n";
        else if(opt->at(i).value_type == ValueType::Char)
            data += opt->at(i).name + "= " +  *(char*)opr[i] + "\n";
        else if(opt->at(i).value_type == ValueType::String)
            data += opt->at(i).name + "= " +  *(QString*)opr[i] + "\n";
    }

    file.write(data.toUtf8().data());
    file.close();
}

void FileOperator::saveWeaponTable(QVector<raw_Weapon> weapon)  // 保存 WeaponTable
{
    qDebug() << "[FileOperator] Saving weapon table...";

    QFile file(FILE_WEAPONTABLE);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "[FileOperator] ERROR: Unable to open file " << FILE_WEAPONTABLE << ".";
        return;
    }

    QString data = FILEHEAD;    // csv BOM
    data += "# 武器Weapon, 图标Icon\n"; // 这是一行注释，真的是一行注释

    for(int i = 0; i < weapon.count(); i++)
    {
        data += weapon.at(i).name + ", " + weapon.at(i).icon.name() + "\n";
    }

    file.write(data.toUtf8().data());
    file.close();
}

void FileOperator::saveEnchantmentTable(QVector<raw_EnchPlus> ench_table) // 保存 EnchantmentTable
{
    qDebug() << "[FileOperator] Saving enchantment table...";

    QFile file(FILE_ENCHTABLE);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "[FileOperator] ERROR: Unable to open file " << FILE_ENCHTABLE << ".";
        return;
    }

    QString data = FILEHEAD;    // csv BOM
    data += "# 魔咒名称, 适用版本(), 最高等级, 附魔台最高等级, 物品乘数, 附魔书乘数, 魔咒冲突列表(竖线“|”分隔), 是否适配物品#1(1:是，0:否), #2, #3, ...\n";    // 这是一行注释

    for(int i = 0; i < ench_table.count(); i++)
    {
        data += ench_table.at(i).name + ", " \
                + QString::number(ench_table.at(i).edition) + ", " \
                + QString::number(ench_table.at(i).max_level) + ", " \
                + QString::number(ench_table.at(i).poor_max_level) + ", " \
                + QString::number(ench_table.at(i).multiplier[0]) + ", " \
                + QString::number(ench_table.at(i).multiplier[1]) + ", ";

        if(ench_table.at(i).repulsion.count() > 0)
        {
            data += ench_table.at(i).repulsion.at(0);
            for(int j = 1; j < ench_table.at(i).repulsion.count(); j++)
                data += " | " + ench_table.at(i).repulsion.at(j);
        }

        if(ench_table.at(i).suitable.count() > 0)
        {
            for(int j = 0; j < ench_table.at(i).suitable.count(); j++)
                data += ", " + QString::number(ench_table.at(i).suitable.at(j));
        }

        data += "\n";
    }

    file.write(data.toUtf8().data());
    file.close();
}


void FileOperator::loadConfig(Config *config, const QVector<Option> *opt) // 加载配置
{
    qDebug() << "[FileOperator] Loading configuration...";

    QFile file(FILE_CONFIG);
    if(!file.open(QIODevice::ReadOnly)) // 若没有配置文件则创建一个默认配置文件
    {
        qDebug() << "[FileOperator] WARNING: file " << FILE_CONFIG << " not found.";
        if(!file.open(QIODevice::WriteOnly))    // 若无法创建则退出
        {
            qDebug() << "[FileOperator] ERROR: Unable to open file " << FILE_CONFIG << ".";
            return;
        }
        file.close();

        saveConfig(*config, opt);   // 创建配置文件
        if(!file.open(QIODevice::ReadOnly)) // 若文件不可读则退出
        {
            qDebug() << "[FileOperator] ERROR: Unable to open file " << FILE_CONFIG << ".";
            return;
        }
        qDebug() << "[FileOperator] File " << FILE_CONFIG << " is created successfully.";
        return; // 创建完成并退出
    }

    QStringList data = QString(file.readAll()).trimmed().split('\n'); // 读取文件内容并除杂切片
    file.close();   // 关闭文件

    bool exist[CONFIG_NUM] = {};
    for(int i = 0; i < data.count(); i++)
    {
        QStringList key__value = data[i].trimmed().split('='); // 进一步除杂并切片

        if(key__value[0][0] == TEXT_NOTE_SYMBOL) // 如果该数据为注释行，则忽略
            continue;
        if(key__value[1].isEmpty()) // 排除空值
            continue;

        if(key__value.count() == 2)  // 若该组数据为键值对则读取内容，否则忽略
        {
            key__value[0] = key__value[0].simplified();  // 对键名深度除杂
            key__value[1] = key__value[1].simplified();  // 对键值深度除杂

            for(int j = 0; j < CONFIG_NUM; j++) // 遍历 config_option ，匹配相应键值对并赋值
            {
                if(key__value[0] == opt->at(j).name)
                {
                    CfgOpr opr;
                    opr.link(config);
                    if(opt->at(i).value_type == ValueType::Bool)
                        *(bool*)opr[j] = (bool)key__value[1].toInt();
                    if(opt->at(i).value_type == ValueType::Int)
                        *(int*)opr[j] = key__value[1].toInt();
                    else if(opt->at(i).value_type == ValueType::Float)
                        *(float*)opr[j] = key__value[1].toFloat();
                    else if(opt->at(i).value_type == ValueType::Double)
                        *(double*)opr[j] = key__value[1].toDouble();
                    else if(opt->at(i).value_type == ValueType::Char)
                        *(char*)opr[j] = key__value[1].toUtf8().data()[0];
                    else if(opt->at(i).value_type == ValueType::String)
                        *(QString*)opr[j] = key__value[1];
                    exist[j] = true;
                    break;
                }
            }
        }
    }

    for(int i = 0; i < CONFIG_NUM; i++) // 检查是否存在未匹配键值对
    {
        if(!exist[i])   // 发现未匹配键值对则立即保存一次配置以更新
        {
            saveConfig(*config, opt);
            break;
        }
    }

    qDebug() << "[FileOperator] Configuration has been loaded!";
}

void FileOperator::loadWeaponTable(QVector<raw_Weapon> *weapon) // 加载 WeaponTable
{
    qDebug() << "[FileOperator] Loading weapon table...";

    QFile file(FILE_WEAPONTABLE);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[FileOperator] WARNING: file " << FILE_WEAPONTABLE << " not found.";
        if(!file.open(QIODevice::ReadWrite))
        {
            qDebug() << "[FileOperator] ERROR: Unable to open" << FILE_WEAPONTABLE << ".";
            return;
        }
        qDebug() << "[FileOperator] File " << FILE_WEAPONTABLE << " is created successfully.";
    }
    QStringList raw_data = QString(file.readAll()).trimmed().replace("\"", "").split('\n'); // 读取文件内容并除杂切片
    file.close();

    QStringList data;
    for(int i = 0; i < raw_data.count(); i++)
    {
        raw_data[i] = raw_data[i].simplified();    // 深度除杂

        if(raw_data[i].isEmpty()) // 排除空行
            continue;
        if(raw_data[i][0] == TEXT_NOTE_SYMBOL)  // 排除注释行
            continue;

        data << raw_data[i];
    }

    if(data.count() == 0)   // 如果没有内容则退出
    {
        qDebug() << "[FileOperator] The weapon table is empty!";
        return;
    }

    for(int i = 0; i < data.count(); i++)
    {
        QStringList values = data[i].split(','); // 对行切片，只使用前两项值
        raw_Weapon tm;
        tm.name = values[0];
        tm.icon = QIcon(values[1]);
        weapon->append(tm);
    }

    qDebug() << "[FileOperator] Weapon table has been loaded!";
}

void FileOperator::loadEnchantmentTable(QVector<raw_EnchPlus> *ench_table)    // 加载 EnchantmentTable
{
    qDebug() << "[FileOperator] Loading enchantment table...";

    QFile file(FILE_ENCHTABLE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "[FileOperator] WARNING: " <<  FILE_ENCHTABLE << " not found.";
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << "[FileOperator] ERROR: Unable to open file " << FILE_ENCHTABLE << ".";
            return;
        }
        qDebug() << "[FileOperator] File " << FILE_ENCHTABLE << " is created successfully.";
    }
    QStringList raw_data = QString(file.readAll()).trimmed().replace("\"", "").split('\n'); // 读取文件内容并除杂切片
    file.close();

    QStringList data;
    for(int i = 0; i < raw_data.count(); i++)
    {
        raw_data[i] = raw_data[i].simplified();    // 深度除杂

        if(raw_data[i].isEmpty()) // 排除空行
            continue;
        if(raw_data[i][0] == TEXT_NOTE_SYMBOL)  // 排除注释行
            continue;

        data << raw_data[i];
    }

    if(data.count() == 0)   // 如果没有内容则退出
    {
        qDebug() << "[FileOperator] The weapon table is empty!";
        return;
    }

    for(int i = 0; i < data.count(); i++)   // 加载数据
    {
        QStringList values = data[i].split(','); // 对行切片，只使用前两项值
        if(values.count() < MINIMUM_EP_ITEM_C)  // 跳过不完整数据行
            continue;

        raw_EnchPlus tm;
        tm.name = values[0];
        tm.edition = (MCE)values[1].toInt();
        tm.max_level = values[2].toInt();
        tm.poor_max_level = values[3].toInt();
        tm.multiplier[0] = values[4].toInt();
        tm.multiplier[1] = values[5].toInt();

        QStringList vdata = values[6].split('|');   // 小切片
        for(int j = 0; j < vdata.count(); j++)
            tm.repulsion.append(vdata[j]);

        for(int j = 7; j < values.count(); j++)   // 可扩展
            tm.suitable.append((bool)values[i].toInt());

        ench_table->append(tm);
    }

    for(int i = 0; i < ench_table->count(); i++)    // 数据有效性校验
    {
        if(ench_table->at(i).name.isEmpty() || ench_table->at(i).edition < 0 ||
                ench_table->at(i).max_level == 0 || ench_table->at(i).poor_max_level < 0 ||
                ench_table->at(i).multiplier[0] == 0 || ench_table->at(i).multiplier[1] == 0)   // 删除无效数据
        {
            ench_table->remove(i);
            i--;
        }
    }
    ench_table->squeeze();  // 释放空闲内存

    qDebug() << "[FileOperator] Enchantment table has been loaded!";
}


void FileOperator::saveExport(const Config config, const QVector<raw_EnchPlus> reps, const Summary summary, const QVector<FlowStep> flow)  // 保存输出结果
{
    qDebug() << "[FileOperator] Saving exportation...";

    QString dir_str;
    if(config.export_path.isEmpty())    // 检查相应配置项，若未配置则使用默认值
        dir_str = DIR_EXPORT;
    else
        dir_str = config.export_path;

    QDir dir;
    if(!dir.exists(dir_str))    // 若输出路径不存在，则创建它
    {
        if(!dir.mkpath(dir_str))    // 若创建失败则退出
        {
            qDebug() << "[FileOperator] ERROR: Unable to access the path " << dir_str << ".";
            return;
        }
    }

    if(flow.count() < 1)    // 不输出空流程
    {
        qDebug() << "[FileOperator] No export, because the flow is empty.";
        return;
    }

    QString name = dir_str + "output_" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmsszzz") + ".txt";  // 文件名
    QFile file(name);
    if(!file.open(QIODevice::WriteOnly))   // 创建文件
    {
        qDebug() << "[FileOperator] ERROR: Unable to output the file!";
        return;
    }

    QString data = name + "\n";

    // 输出流程概要
    for(int i = 0; i < name.count(); i++)
        data += '-';
    data += "\nEdition: " + QString::number(summary.edition);
    data += "\nInput: " + summary.input_item.name.simplified();
    if(summary.input_item_ench.count() > 0)
    {
        for(int i = 0; i < summary.input_item_ench.count(); i++)
        {
            raw_EnchPlus rep = reps.at(summary.input_item_ench.at(i).id);
            data += "\n    " + rep.name.simplified() + " [" + QString::number(summary.input_item_ench.at(i).lvl) + "]";
        }
    }
    data += "\nOutput: " + summary.output_item.name.simplified();
    if(summary.output_item_ench.count() > 0)
    {
        for(int i = 0; i < summary.output_item_ench.count(); i++)
        {
            raw_EnchPlus rep = reps.at(summary.output_item_ench.at(i).id);
            data += "\n    " + rep.name.simplified() + " [" + QString::number(summary.output_item_ench.at(i).lvl) + "]";
        }
    }
    data += "\nLevel Cost: " + QString::number(summary.level_cost);
    data += "\nPoint Cost: " + QString::number(summary.point_cost);
    data += "\nStep Count: " + QString::number(summary.step_count);
    data += "\nProcessable: " + QString(summary.processable? "Yes": "No");
    for(int i = 0; i < name.count(); i++)
        data += '-';
    data += "\n";

    // 输出具体流程
    for(int i = 0; i < flow.count(); i++)
    {
        data += "\n[" + QString::number(i) + "]A: " + QString(flow.at(i).a.type < 0? ID_ECB: summary.input_item.name.simplified());    // 步骤序号+目标物品名称
        for(int j = 0; j < flow.at(i).a.ench.count(); j++)  // 目标物品魔咒
        {
            raw_EnchPlus rep = reps.at(flow.at(i).a.ench.at(j).id);
            data += "\n    " + rep.name.simplified() + " [" + QString::number(flow.at(i).a.ench.at(j).lvl) + "]";
        }

        data += "\n[" + QString::number(i) + "]B: " + QString(flow.at(i).a.type < 0? ID_ECB: summary.input_item.name.simplified());    // 步骤序号+牺牲物品名称
        for(int j = 0; j < flow.at(i).a.ench.count(); j++)  // 牺牲物品魔咒
        {
            raw_EnchPlus rep = reps.at(flow.at(i).a.ench.at(j).id);
            data += "\n    " + rep.name.simplified() + " [" + QString::number(flow.at(i).a.ench.at(j).lvl) + "]";
        }

        data += "\n";   // 换行
    }


    file.write(data.toUtf8().data());
    file.close();

    qDebug() << "[FileOperator] Finished!";
}


Anvil::Anvil(MCE mce, PFADDN add, const QVector<EnchPlus> * ep)
{
    edition = mce;
    addition = add;
    eps = ep;
}

bool Anvil::checkRepulsed(const Ench a, const Ench b) // 检查是否存在魔咒冲突，魔咒与魔咒
{
    EnchPlus ep = eps->at(a.id);
    int ec = ep.repulsion.count();

    for(int i = 0; i < ec; i++)
    {
        if(ep.repulsion.at(i) == b.id)
            return true;
    }

    return false;
}

bool Anvil::checkRepulsed(const Ench e, const Item it) // 检查是否存在魔咒冲突，魔咒与物品
{
    EnchPlus ep = eps->at(e.id);
    int ec = ep.repulsion.count();
    int iec = it.ench.count();

    for(int i = 0; i < ec; i++)
    {
        for(int j = 0; j < iec; j++)
        {
            if(ep.repulsion.at(i) == it.ench.at(j).id)
                return true;
        }
    }

    return false;
}

int Anvil::preforge(const Item a, Item b) // 花费计算
{
    if(edition == MCE::All)    // 排除不支持的mode
        return NULL;

    int cost = 0;
    int a_ec = a.ench.count();  // 目标物品的魔咒数
    int b_ec = b.ench.count();  // 牺牲物品的魔咒数
    int multi = b.type < 0? 1: 0;  // 选择乘数


    // 处理冲突的魔咒
    int replc = 0;  // 暂存冲突花费
    if(a_ec > 0)
    {
        for(int i = 0; i < b_ec; i++)
        {
            if(checkRepulsed(b.ench.at(i), a))
            {
                if(edition == MCE::Java)   // JE中每一项冲突花费+1
                    replc += 1;
                b.ench.remove(i);   // 删除冲突的魔咒
                i--;
            }
        }
    }
    b_ec = b.ench.count();  // 更新魔咒数



    // 计算魔咒等级花费
    for(int i = 0; i < b_ec; i++)
    {
        EnchPlus bep = eps->at(b.ench.at(i).id);   // 拉取魔咒表对应数据
        if(a_ec > 0)
        {
            int p = -1;
            for(int j = 0; j < a.ench.count(); j++) // 查找是否存在两个相同的可合并魔咒
            {
                if(a.ench.at(j).id == b.ench.at(i).id)
                {
                    p = j;
                    break;
                }
            }

            if(p < 0)   // 不存在
                cost += bep.multiplier[multi] * b.ench.at(i).lvl;
            else    // 存在
            {
                if(edition == MCE::Java)
                    cost += bep.multiplier[multi] * std::min(forge(a.ench.at(p).lvl, b.ench.at(i).lvl), bep.max_level);
                else
                    cost += bep.multiplier[multi] * (std::min(forge(a.ench.at(p).lvl, b.ench.at(i).lvl), bep.max_level) - a.ench.at(p).lvl);
            }
        }
        else
            cost += bep.multiplier[multi] * b.ench.at(i).lvl;
    }


    // 计算其它项
    switch(addition) {
    case PFADDN::Normal:
        cost += replc;  // 计入冲突花费
        cost += pow(2, a.penalty) + pow(2, b.penalty) -2; // 计算累积惩罚
        if(a.durability < 100 && b.durability > 0)  // 计算维修费用
            cost += 2;
        break;
    case PFADDN::NoRepair:
        cost += replc;  // 计入冲突花费
        cost += pow(2, a.penalty) + pow(2, b.penalty) -2; // 计算累积惩罚
        break;
    case PFADDN::NoRepRepulsion:
        cost += replc;  // 计入冲突花费
        break;
    case PFADDN::Extreme:
        break;
    default:
        return -1;
        break;
    }

    return cost;
}

int Anvil::forge(int a, int b) // 等级合并（不计上限）
{
    if(a == b)
        a++;
    else
        a = std::max(a, b);

    return a;
}

Item Anvil::forge(const Item a, Item b)  // 物品合并
{
    Item tm = a;

    int a_ec = a.ench.count();  // 目标物品的魔咒数
    int b_ec = b.ench.count();  // 牺牲物品的魔咒数

    // 处理冲突的魔咒
    if(a_ec > 0)
    {
        for(int i = 0; i < b_ec; i++)
        {
            if(checkRepulsed(b.ench.at(i), a))
            {
                b.ench.remove(i);   // 删除冲突的魔咒
                i--;
            }
        }
    }
    b_ec = b.ench.count();  // 更新魔咒数


    // 合并物品
    for(int i = 0; i < b_ec; i++)
    {
        EnchPlus bep = eps->at(b.ench.at(i).id);   // 拉取魔咒表对应数据
        if(a_ec > 0)
        {
            int p = -1;
            for(int j = 0; j < a.ench.count(); j++) // 查找是否存在两个相同的可合并魔咒
            {
                if(a.ench.at(j).id == b.ench.at(i).id)
                {
                    p = j;
                    break;
                }
            }

            if(p < 0)   // 不存在
                tm.ench.append(b.ench.at(i));
            else    // 存在
            {
                Ench e = b.ench.at(i);
                e.lvl = std::min(forge(a.ench.at(p).lvl, b.ench.at(i).lvl), bep.max_level); // 合并等级
                tm.ench.append(e);
            }
        }
        else
            tm.ench.append(b.ench.at(i));
    }

    return tm;  // 返回合并后的物品
}

int Anvil::compare(const Item a, Item b)
{
    return preforge(Item(), a) - preforge(Item(), b);
}

FlowStep Anvil::combine(const Item a, Item b)
{
    int cost = preforge(a, b);  // 计算合并花费
    if(cost < 0)
        return FlowStep();

    FlowStep fs;
    fs.a = a;
    fs.b = b;
    fs.levelCost = cost;

    return fs;
}


EnchFilter::EnchFilter()
{

}

ItemFilter::ItemFilter()
{

}


Transformer::Transformer(const QVector<raw_Weapon> * rwp, const QVector<raw_EnchPlus> * rep)
{
    rwps = rwp;
    reps = rep;
}

ItemPro Transformer::operator=(const Item* it)
{
    ItemPro itp;

    if(it->type < 0)    // 转换图标和名称
    {
        itp.icon = ICON_ECB;
        itp.name = ID_ECB;
    }
    else
    {
        itp.icon = rwps->at(it->type).icon;
        itp.name = rwps->at(it->type).name;
    }

    QString data = "";
    if(it->ench.count() > 0)    // 转换魔咒信息
    {
        data += reps->at(it->ench.at(0).id).name + " " + numToRoman(it->ench.at(0).lvl);
        for(int i = 1; i < it->ench.count(); i++)
            data += "\n" + reps->at(it->ench.at(i).id).name + " " + numToRoman(it->ench.at(i).lvl);
    }
    itp.ench_set = data;

    return itp;
}

FlowStepPro Transformer::operator=(const FlowStep* fs)
{
    FlowStepPro fsp;
    fsp.a = Transformer::operator=(&fs->a);
    fsp.b = Transformer::operator=(&fs->b);
    fsp.levelCost = fs->levelCost;

    if(fs->levelCost <= 16)  // 计算经验值花费
        fsp.pointCost = (fs->levelCost * fs->levelCost + 6 * fs->levelCost);
    else if(fs->levelCost <= 31)
        fsp.pointCost = (2.5 * fs->levelCost * fs->levelCost - 40.5 * fs->levelCost + 360);
    else
        fsp.pointCost = (4.5 * fs->levelCost * fs->levelCost - 162.5 * fs->levelCost + 2220);

    return fsp;
}


void deliverID(QVector<raw_EnchPlus> *reps, QVector<raw_Weapon> *rwps, QVector<EnchPlus> *eps, QVector<Weapon> *wps)    // 分配魔咒和Weapon的数字ID
{
    // 魔咒的数字ID
    int retc = reps->count();

    for(int i = 0; i < retc; i++)
    {
        EnchPlus tm;
        tm.id = i;
        tm.edition = reps->at(i).edition;
        tm.poor_max_level = reps->at(i).poor_max_level;
        tm.max_level = reps->at(i).max_level;
        tm.multiplier[0] = reps->at(i).multiplier[0];
        tm.multiplier[1] = reps->at(i).multiplier[1];
        eps->append(tm);
    }

    for(int i = 0; i < retc; i++)
    {
        int repc = reps->at(i).repulsion.count();
        for(int j = 0; j < repc; j++)
        {
            for(int k = 0; k < retc; k++)
            {
                if(reps->at(i).repulsion.at(j) == reps->at(k).name)
                {
                    (*eps)[i].repulsion.append(k);
                    break;
                }
            }
        }
    }


    // Weapon的数字ID
    int wpc = rwps->count();

    for(int i = 0; i < wpc; i++)
    {
        Weapon tm;
        tm.id = i;
        wps->append(tm);
    }

    for(int i = 0; i < retc; i++)
    {
        for(int j = 0; j < wpc; j++)
        {
            if(reps->at(i).suitable.at(j))
            {
                (*wps)[j].suitableE.append(eps->at(i));
            }
        }
    }
}


QString numToRoman(int num) // 阿拉伯数字转罗马数字
{
    if(num > 10 || num < 1)
        return "level." + QString::number(num);
    else if(num == 10)
        return "X";
    else if(num == 9)
        return "IX";
    else if(num == 4)
        return "IV";
    QString str = "";
    if(num > 4)
    {
        str += "V";
        for(int i = num%5; i > 0; i--)
            str += "I";
    }
    else if(num < 4)
    {
        for(int i = num; i > 0; i--)
            str += "I";
    }
    return str;
}

