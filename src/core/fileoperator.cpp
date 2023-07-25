#include "fileoperator.h"

FileOperator::FileOperator(Core *c, QObject *parent)
    : QObject{parent},
      mdb("FileOperator")
{
    core = c;
}

// 加载、导入数据
void FileOperator::loadConfig()
{
    mdb.msg("加载配置中...");

    // 读取文件数据
    QFile file(FILE_CONFIG);
    QString raw;
    bool b = readFile(&file, &raw, stringFoil(core->config));

    // 判定数据读取是否成功
    if(b)
    {
        // 数据预处理
        QList<QStringList> data = StringKnife::crossSlice(raw, '\n', '=', 2);
        if(data.last().at(0).startsWith(TEXT_NOTE_SYMBOL))  // 处理文件版本信息
        {
            data.takeLast();
            // *配置文件无文件版本信息
        }

        // 载入数据
        int n[CONFIG_NUM] = {};
        int k = 0;
        for(int i = 0; i < data.count(); i++)
        {
            QString key = data.at(i).at(0);
            QString value = data.at(i).at(1);

            if(key == "config_version")
            {
                core->config.config_version = value.toInt();
                n[0]++;
            }
            else if(key == "default_edition")
            {
                if((MCE)value.toInt() == MCE::Java || (MCE)value.toInt() == MCE::Bedrock)
                {
                    core->config.default_edition = (MCE)value.toInt();
                    n[1]++;
                }
            }
            else if(key == "default_itemconfig")
            {
                core->config.default_itemconfig = (ICM)value.toInt();
                n[2]++;
            }
            else if(key == "default_algorithm")
            {
                core->config.default_algorithm = (ALGM)value.toInt();
                n[3]++;
            }
            else if(key == "export_path")
            {
                core->config.export_path = value;
                n[4]++;
            }
            else if(key == "auto_save")
            {
                core->config.auto_save = value.toInt();
                n[5]++;
            }
            else if(key == "enable_custom_en")
            {
                core->config.enable_custom_en = value.toInt();
                n[6]++;
            }
            else if(key == "enable_custom_we")
            {
                core->config.enable_custom_we = value.toInt();
                n[7]++;
            }
            else if(key == "auto_check_update")
            {
                core->config.auto_check_update = value.toInt();
                n[8]++;
            }
            else if(key == "enable_reszie_window")
            {
                core->config.enable_reszie_window = value.toInt();
                n[9]++;
            }
            else if(key == "deverloper_mode")
            {
                core->config.deverloper_mode = value.toInt();
                n[10]++;
            }

            // 记录各配置项的读取情况
            if(i >= CONFIG_NUM-1)
            {
                k = 0;
                for(int j = 0; j < CONFIG_NUM; j++)
                {
                    if(n[j] > 0)
                        k++;
                }
                if(k == CONFIG_NUM) // 抛弃多余的项
                    break;
            }
        }

        // 判断是否进行过版本更新
        if(core->config.config_version < VERSION_ID || n[0] == 0)
        {
            core->is_on_update = true;
            core->config.config_version = VERSION_ID;
        }

        // 检查配置完整性
        if(k == CONFIG_NUM && data.count() == CONFIG_NUM)
        {
            mdb.msg("配置加载成功！");
        }
        else
        {
            saveConfig();   // 用现有有效数据和默认数据覆盖不正确或缺失的项
            mdb.msg("配置加载成功，并修正了其中的问题！");
        }
    }
    else
    {
        mdb.msg(warning, "未能读取配置，正在使用默认配置值！");
    }
}

void FileOperator::loadWeapons()
{
    mdb.msg("加载武器池中...");

    // 读取文件数据
    QFile file(FILE_WEAPONTABLE);
    QString raw;
    bool b = readFile(&file, &raw, FILEHEAD_CSV);
    if(raw.isEmpty())
    {
        mdb.msg(warning, "武器池无效，正在使用默认武器池！");
        return;
    }

    // 判定数据读取是否成功
    if(b)
    {
        // 数据预处理
        QList<QStringList> data = StringKnife::crossSlice(raw, '\n', ',', 2);
        if(data.isEmpty())
        {
            mdb.msg(warning, "武器池无效，正在使用默认武器池！");
            return;
        }
        if(data.last().at(0).startsWith(TEXT_NOTE_SYMBOL))  // 载入文件版本信息
        {
            QString s = data.takeLast().at(0);
            s.remove(0, 1);
            core->weapon_table_file_version = s.toInt();
        }

        // 检查文件版本
        if(core->weapon_table_file_version < FILEVERSION)
        {
            mdb.msg(warning, "文件版本过低，正在使用默认武器池！");
            return;
        }

        // 载入数据
        rwps.clear();
        for(int i = 0; i < data.count(); i++)
        {
            // 数据暂存于本类
            raw_Weapon rwp;
            rwp.name = data.at(i).at(0);
            rwp.icon = QIcon(data.at(i).at(1));
            rwps.append(rwp);
        }

        // 去重
        for(int i = 0; i < rwps.count(); i++)
        {
            for(int j = 0; j < rwps.count(); j++)
            {
                if(i != j && rwps.at(i).name == rwps.at(j).name)
                {
                    rwps.takeAt(std::min(i, j));
                    break;
                }
            }
        }

        mdb.msg("武器池加载完毕.");
    }
    else
    {
        mdb.msg(warning, "未能读取武器池，正在使用默认武器池！");
    }
}

void FileOperator::loadEnchantments()
{
    mdb.msg("加载魔咒池中...");

    // 读取文件数据
    QFile file(FILE_ENCHTABLE);
    QString raw;
    bool b = readFile(&file, &raw, FILEHEAD_CSV);

    // 判定数据读取是否成功
    if(b)
    {
        // 数据预处理
        QList<QStringList> data = StringKnife::multicrossSlice(raw, '\n', ',');
        if(data.isEmpty())
        {
            mdb.msg(warning, "魔咒池无效，正在使用默认魔咒池！");
            return;
        }
        if(data.last().at(0).startsWith(TEXT_NOTE_SYMBOL))  // 载入文件版本信息
        {
            QString s = data.takeLast().at(0);
            s.remove(0, 1);
            core->enchantment_table_file_version = s.toInt();
        }

        // 检查文件版本
        if(core->enchantment_table_file_version < FILEVERSION)
        {
            mdb.msg(warning, "文件版本过低，正在使用默认魔咒池！");
            return;
        }

        // 载入数据
        reps.clear();
        for(int i = 0; i < data.count(); i++)
        {
            // 数据暂存于本类
            if(data.at(i).count() < 8)
                continue;

            raw_EnchPlus rep;
            rep.name = data.at(i).at(0);
            rep.max_level = data.at(i).at(1).toInt();
            rep.poor_max_level = data.at(i).at(2).toInt();
            rep.multiplier[0] = data.at(i).at(3).toInt();
            rep.multiplier[1] = data.at(i).at(4).toInt();
            rep.edition = (MCE)data.at(i).at(5).toInt();
            QStringList data2 = data.at(i).at(6).split('|');
            for(int j = 0; j < data2.count(); j++)
            {
                if(data2.at(j).isEmpty())
                    continue;
                rep.repulsion.append(data2.at(j).trimmed());
            }
            for(int j = 7; j < data.at(i).count(); j++)
            {
                rep.suitable.append(data.at(i).at(j).toUInt());
            }
            reps.append(rep);
        }

        // 去重
        for(int i = 0; i < reps.count(); i++)
        {
            for(int j = 0; j < reps.count(); j++)
            {
                if(i != j && reps.at(i).name == reps.at(j).name && reps.at(i).edition == reps.at(j).edition)
                {
                    reps.takeAt(std::min(i, j));
                    break;
                }
            }
        }

        mdb.msg("魔咒池加载完毕.");
    }
    else
    {
        mdb.msg(warning, "未能读取魔咒池，正在使用默认魔咒池！");
    }
}

void FileOperator::loadFlow()   // 需多版本兼容，变化多，难度较大
{

}

// 保存、导出数据
void FileOperator::saveConfig()
{
    mdb.msg("保存配置中...");

    QFile file(FILE_CONFIG);
    QString data = stringFoil(core->config);
    if(writeFile(&file, data))
    {
        mdb.msg("配置保存成功！");
    }
    else
    {
        mdb.msg(error, QString("配置保存失败！请检查配置文件 ") + FILE_CONFIG + " 是否被锁定.");
    }
}

void FileOperator::saveWeapons()
{
    mdb.msg("保存武器池中...");

    QFile file(FILE_WEAPONTABLE);
    QString data = FILEHEAD_CSV + stringFoil(rwps);
    if(writeFile(&file, data))
    {
        mdb.msg("武器池保存成功！");
    }
    else
    {
        mdb.msg(wrong, QString("武器池保存失败！请检查文件 ") + FILE_WEAPONTABLE + " 是否被锁定.");
    }
}

void FileOperator::saveEnchantments()
{
    mdb.msg("保存魔咒池中...");

    QFile file(FILE_ENCHTABLE);
    QString data = FILEHEAD_CSV + stringFoil(reps);
    if(writeFile(&file, data))
    {
        mdb.msg("魔咒池保存成功！");
    }
    else
    {
        mdb.msg(wrong, QString("魔咒池保存失败！请检查文件 ") + FILE_ENCHTABLE + " 是否被锁定.");
    }
}

void FileOperator::saveFlow()
{
    mdb.msg("保存流程中...");

    QString path;   // 自定义输出路径
    QString c_date_time = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz");
    QDir dir(core->config.export_path);
    if(dir.exists())
    {
        path = core->config.export_path;
        if(path.last(1) == "\\") // 检查末端斜杠
            path.chop(1);
        path += "\\output_" + c_date_time + ".csv";
    }
    else
        path = "output_" + c_date_time + ".csv";

    QFile file(path);
    QString data = FILEHEAD_CSV + stringFoil(core->summary, core->flow, c_date_time);
    if(writeFile(&file, data))
    {
        mdb.msg("流程保存成功！");
    }
    else
    {
        mdb.msg(wrong, "流程保存失败！");
    }
}

// 信息转换
void FileOperator::updateToCore()
{
    mdb.msg("解析池数据中...");
    updateToLocal(true);

    QVector<EnchPlus> eps;
    for(int i = 0; i < reps.count(); i++)   // 魔咒池数据转换
    {
        EnchPlus ep;
        ep.name = reps.at(i).name;
        ep.edition = reps.at(i).edition;
        ep.max_level = reps.at(i).max_level;
        ep.poor_max_level = reps.at(i).poor_max_level;
        ep.multiplier[0] = reps.at(i).multiplier[0];
        ep.multiplier[1] = reps.at(i).multiplier[1];
        for(int j = 0; j < reps.at(i).repulsion.count(); j++)
        {
            ep.repulsion.append({reps.at(i).repulsion.at(j), 0});
        }
        eps.append(ep);
    }

    QVector<WeaponPlus> wps;
    for(int i = 0; i < rwps.count(); i++)   // 武器池数据转换
    {
        WeaponPlus wp;
        wp.name = rwps.at(i).name;
        wp.icon = rwps.at(i).icon;
        for(int j = 0; j < reps.count(); j++)
        {
            if(reps.at(j).suitable.count()-1 < i)   // 处理 suitable 的数据缺失
                continue;
            if(reps.at(j).suitable.at(i))
            {
                wp.suitableE.append(eps.at(j));
            }
        }
        wps.append(wp);
    }

    if(wps.count() * eps.count() == 0)
    {
        mdb.msg(wrong, "数据解析失败，数据无效!");
    }
    else
    {
        core->weapon_table.clear();
        core->weapon_table = wps;
        core->ench_table.clear();
        core->ench_table = eps;

        mdb.msg("数据解析成功!");
    }
}

void FileOperator::updateToLocal(bool protect)
{
    mdb.msg("下载池数据中...");

    if(core->weapon_table.count() * core->ench_table.count() == 0)  // 检查数据完整性
    {
        mdb.msg(error, "无法下载池数据，数据错误");
        return;
    }

    if(!protect)
    {
        rwps.clear();
        reps.clear();
    }

    if(rwps.isEmpty())
    {
        for(int i = 0; i < core->weapon_table.count(); i++)   // 武器池数据转换
        {
            raw_Weapon rwp;
            rwp.name = core->weapon_table.at(i).name;
            rwp.icon = core->weapon_table.at(i).icon;
            rwps.append(rwp);
        }
    }

    if(reps.isEmpty())
    {
        for(int i = 0; i < core->ench_table.count(); i++)   // 魔咒池数据转换
        {
            raw_EnchPlus rep;
            rep.name = core->ench_table.at(i).name;
            rep.edition = core->ench_table.at(i).edition;
            rep.max_level = core->ench_table.at(i).max_level;
            rep.poor_max_level = core->ench_table.at(i).poor_max_level;
            rep.multiplier[0] = core->ench_table.at(i).multiplier[0];
            rep.multiplier[1] = core->ench_table.at(i).multiplier[1];
            for(int j = 0; j < core->ench_table.at(i).repulsion.count(); j++)
            {
                rep.repulsion.append(core->ench_table.at(i).repulsion.at(j));
            }
            for(int j = 0; j < core->weapon_table.count(); j++) // 遍历武器池
            {
                for(int k = 0; k < core->weapon_table.at(j).suitableE.count()+1; k++)   // 搜寻匹配魔咒
                {
                    if(k > core->weapon_table.at(j).suitableE.count()-1)
                    {
                        rep.suitable.append(false);
                        break;
                    }
                    if(rep.name == core->weapon_table.at(j).suitableE.at(k).name)
                    {
                        rep.suitable.append(true);
                        break;
                    }
                }
            }
            reps.append(rep);
        }
    }

    if(rwps.count() * reps.count() == 0)
    {
        mdb.msg(error, "数据下载失败，数据错误!");
    }
    else
    {
        mdb.msg("数据下载完成!");
    }
}


// 文件读写
bool readFile(QFile *file, QString *raw, QString default_data)
{
    MDebug mdb("f_QuickFileReader");
    mdb.msg("读取文件中...");
    if(!file->open(QIODevice::ReadOnly)) // 若没有配置文件则创建一个默认配置文件
    {
        mdb.msg(warning, "找不到文件 " + file->fileName() + ".");
        if(!file->open(QIODevice::WriteOnly))    // 若无法创建则退出
        {
            mdb.msg(error, "无法打开文件 " + file->fileName() + ".");
            return false;
        }
        if(default_data != "")
            file->write(default_data.toUtf8().data());   // 写入预置数据
        file->close();   // 关闭文件

        mdb.msg(info, "文件 " + file->fileName() + " 新建成功，但没有数据被读取.");
        return false; // 创建完成并退出
    }

    *raw = file->readAll(); // 读取文件内容
    file->close();   // 关闭文件
    mdb.msg("文件读取完成.");
    return true;
}

bool writeFile(QFile *file, QString str)
{
    MDebug mdb("f_QuickFileWriter");
    mdb.msg("写入文件中...");
    if(!file->open(QIODevice::WriteOnly))    // 若无法打开文件则退出
    {
        mdb.msg(error, "无法打开文件 " + file->fileName() + ".");
        return false;
    }

    file->write(str.toUtf8().data());    // 写入数据
    file->close();   // 关闭文件
    mdb.msg("文件写入完成.");
    return true;
}

bool writeFile(QFile *file, QStringList strs)
{
    MDebug mdb("f_QuickFileWriter");
    mdb.msg("写入文件中...");
    if(!file->open(QIODevice::WriteOnly))    // 若无法打开文件则退出
    {
        mdb.msg(error, "无法打开文件 " + file->fileName() + ".");
        return false;
    }

    for(int i = 0; i < strs.count(); i++)
    {
        file->write(strs.at(i).toUtf8().data());    // 写入数据
        if(i < strs.count()-1)
            file->write("\n");
    }
    file->close();   // 关闭文件
    mdb.msg("文件写入完成.");
    return true;
}


// 数据转换 <Type> -> QString
QString stringFoil(QStringList strs)
{
    // 拼接列表内数据
    QString str;
    for(int i = 0; i < strs.count(); i++)
        str += strs.at(i);
    return str;
}

QString stringFoil(Config cfg)
{
    QString str = "# 配置文件\n";
    str += QString("config_version= ") + QString::number(cfg.config_version) + "\n";
    str += QString("default_edition= ") + QString::number(cfg.default_edition) + "\n";
    str += QString("default_itemconfig= ") + QString::number(cfg.default_itemconfig) + "\n";
    str += QString("default_algorithm= ") + QString::number(cfg.default_algorithm) + "\n";
    str += QString("export_path= ") + cfg.export_path + "\n";
    str += QString("auto_save= ") + QString::number(cfg.auto_save) + "\n";
    str += QString("auto_check_update= ") + QString::number(cfg.auto_check_update) + "\n";
    str += QString("enable_custom_we= ") + QString::number(cfg.enable_custom_we) + "\n";
    str += QString("enable_custom_en= ") + QString::number(cfg.enable_custom_en) + "\n";
    str += QString("enable_reszie_window= ") + QString::number(cfg.enable_reszie_window) + "\n";
    str += QString("deverloper_mode= ") + QString::number(cfg.deverloper_mode);
    return str;
}

QString stringFoil(QVector<raw_EnchPlus> reps)
{
    QString str = "#? file_version= " + QString::number(FILEVERSION) + "\n";    // 写入文件版本信息
    str += "# 魔咒池";
    for(int i = 0; i < reps.count(); i++)
    {
        str += reps.at(i).name + ",";
        str += QString::number(reps.at(i).max_level) + ",";
        str += QString::number(reps.at(i).poor_max_level) + ",";
        str += QString::number(reps.at(i).multiplier[0]) + ",";
        str += QString::number(reps.at(i).multiplier[1]) + ",";
        str += QString::number(reps.at(i).edition) + ",";
        for(int j = 0; j < reps.at(i).repulsion.count(); j++)   // 魔咒冲突
        {
            str += reps.at(i).repulsion.at(j);
            if(j < reps.at(i).repulsion.count()-1)
                str += "|";
            else
                str += ",";
        }
        for(int j = 0; j < reps.at(i).suitable.count(); j++)    // 魔咒适配
        {
            str += QString::number(reps.at(i).suitable.at(j));
            if(j < reps.at(i).repulsion.count()-1)
                str += ",";
        }
        if(i < reps.count()-1)
            str += "\n";
    }
    return str;
}

QString stringFoil(QVector<raw_Weapon> rwps)
{
    QString str = "#? file_version= " + QString::number(FILEVERSION) + "\n";    // 写入文件版本信息
    str += "# 武器池";
    for(int i = 0; i < rwps.count(); i++)
    {
        str += rwps.at(i).name + ",";
        str += rwps.at(i).icon.name();
        if(i < rwps.count()-1)
            str += "\n";
    }
    return str;
}

QString stringFoil(Summary summary, QVector<FlowStep> fs, QString addition)
{
    QString str = "#? file_version= " + QString::number(FILEVERSION) + "\n";    // 写入文件版本信息
    str += "# 附魔流程," + addition + "\n";

    // 写入摘要信息
    str += "\n适用的游戏编译版本:,"; // 编译版本
    if(summary.edition == MCE::All)
        str += "Java Edition & Bedrock Edition,";
    else if(summary.edition == MCE::Java)
        str += "Java Edition (JE),";
    else if(summary.edition == MCE::Bedrock)
        str += "Bedrock Edition (BE),";
    else
        str += "未知 (Unknow),";
    str += "算法代号:," + QString::number(VERSION_ID) + "#" + QString::number(summary.algorithm) + "\n";    // 算法信息
    str += "计算条件:," + QString::number(VERSION_ID) + "#" + QString::number(summary.pfaddn) + "\n";    // 条件信息

    str += "输入物品:,<" + summary.input_item.name + ">[" + QString::number(summary.input_item.penalty) + "],"; // 输入物品
    for(int i = 0; i < summary.input_item.ench.count(); i++)
    {
        str += summary.input_item.ench.at(i).name + " " + numToRoman(summary.input_item.ench.at(i).lvl);
        if(i < summary.input_item.ench.count()-1)
            str += ",";
    }
    str += "\n";
    str += "输出物品:,<" + summary.output_item.name + ">[" + QString::number(summary.output_item.penalty) + "],";   // 输出物品
    for(int i = 0; i < summary.output_item.ench.count(); i++)
    {
        str += summary.output_item.ench.at(i).name + " " + numToRoman(summary.output_item.ench.at(i).lvl);
        if(i < summary.output_item.ench.count()-1)
            str += ",";
    }

    str += "\n步骤数:," + QString::number(summary.step_count) + "," + "总经验等级花费:," + QString::number(summary.level_cost) + "," + "总经验值花费:," + QString::number(summary.point_cost) + "\n\n"; // 步骤 & 花费

    // 写入流程信息
    for(int i = 0; i < fs.count(); i++)
    {
        str += ",等级花费," + QString::number(fs.at(i).levelCost) + "," + "经验值花费," + QString::number(fs.at(i).pointCost) + "\n";    // 花费
        str += QString("A") + "," + "<" + fs.at(i).a.name + ">[" + QString::number(fs.at(i).a.penalty) + "]" + ","; // 目标物品
        for(int j = 0; j < fs.at(i).a.ench.count(); j++)
        {
            str += fs.at(i).a.ench.at(j).name + " " + numToRoman(fs.at(i).a.ench.at(j).lvl);
            if(j < fs.at(i).a.ench.count()-1)
                str += ",";
        }
        str += "\n";
        str += QString("B") + "," + "<" + fs.at(i).b.name + ">[" + QString::number(fs.at(i).b.penalty) + "]" + ","; // 牺牲物品
        for(int j = 0; j < fs.at(i).b.ench.count(); j++)
        {
            str += fs.at(i).b.ench.at(j).name + " " + numToRoman(fs.at(i).b.ench.at(j).lvl);
            if(j < fs.at(i).b.ench.count()-1)
                str += ",";
        }
        if(i < fs.count()-1)
            str += "\n";
    }

    str += "# 流程结束";
    return str;
}

