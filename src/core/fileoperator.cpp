#include "fileoperator.h"
#include <QDebug>

bool FileOperator::loadConfig(QString file_name) // 加载配置
{
    DEBUG("FileOperator", "Loading configuration...", "info");
    QFile f(file_name);
    QSettings settings(file_name, QSettings::IniFormat);

    if (f.open(QIODevice::ReadOnly))
    {
        //        DEBUG("FileOperator", file_name + " found", "info");
        if (!settings.contains("version/file_version"))
            return false;
        if (!settings.contains("version/app_version"))
            return false;

        SettingsMap settings_map;
        settings_map["version/file_version"] = settings.value("version/file_version", 0);
        settings_map["version/app_version"] = settings.value("version/app_version", 0);
        settings_map["default/edition"] = settings.value("default/edition", 1);
        settings_map["default/item_config"] = settings.value("default/item_config", 0);
        settings_map["default/algorithm"] = settings.value("default/algorithm", "None");
        settings_map["default/export_path"] = settings.value("default/export_path", "./exports");
        settings_map["default/language"] = settings.value("default/language", "zh_CN");
        settings_map["lever/auto_save"] = settings.value("lever/auto_save", 0);
        settings_map["lever/auto_check_update"] = settings.value("lever/auto_check_update", 1);
        settings_map["lever/enable_widely_reszie_window"] = settings.value("lever/enable_widely_reszie_window", 0);
        settings_map["log/last_used_table"] = settings.value("log/last_used_table", "");
        settings_map["log/last_edit"] = QDateTime::fromString(settings.value("log/last_edit", "").toString(), "yyyyMMdd_hhmmss");
        global_settings.fromSettingsMap(settings_map);

        DEBUG("FileOperator", tr("Configuration loaded"), "info");
        f.close();
        return true;
    }
    else
    {
        DEBUG("FileOperator", "Unable to open" + file_name, "error");
        return false;
    }
}

bool FileOperator::saveConfig(QString file_name) // 保持配置
{
    DEBUG("FileOperator", "Saving configuration...", "info");
    QFile f(file_name);
    QSettings settings(file_name, QSettings::IniFormat);

    if (f.open(QIODevice::WriteOnly))
    {
        //        DEBUG("FileOperator", file_name + " found.", "info");
        SettingsMap settings_map = global_settings.toSettingsMap();
        settings.clear();
        settings.setValue("version/file_version", FILEVERSION);
        settings.setValue("version/app_version", VERSION_ID);
        settings.setValue("default/edition", settings_map["default/edition"]);
        settings.setValue("default/item_config", settings_map["default/item_config"]);
        settings.setValue("default/algorithm", settings_map["default/algorithm"]);
        settings.setValue("default/export_path", settings_map["default/export_path"]);
        settings.setValue("default/language", settings_map["default/language"]);
        settings.setValue("lever/auto_save", settings_map["lever/auto_save"]);
        settings.setValue("lever/auto_check_update", settings_map["lever/auto_check_update"]);
        settings.setValue("lever/enable_widely_reszie_window", settings_map["lever/enable_widely_reszie_window"]);
        settings.setValue("log/last_used_table", settings_map["log/last_used_table"]);
        settings.setValue("log/last_edit", QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
        DEBUG("FileOperator", "Configuration saved", "info");

        f.close();
        return true;
    }
    else
    {
        DEBUG("FileOperator", "Unable to open" + file_name, "error");
        return false;
    }
}

bool FileOperator::loadTableData(QString file_name, bool m)
{
    DEBUG("FileOperator", "Loading table data...", "info");
    QFile f(file_name);

    if (f.open(QIODeviceBase::ReadOnly)) // 只读模式打开文件
    {
        QFileInfo f_info(f); // 获取文件信息

        // 区分文件格式
        if (f_info.suffix().toLower() == "json") // JSON Format
        {
            //            DEBUG("FileOperator", "JSON Table", "info");
            QJsonParseError json_err;
            QJsonDocument json_doc = QJsonDocument::fromJson(f.readAll(), &json_err); // 读取文件全部数据并写入 JsonDocument 对象
            f.close();                                                                // 关闭文件

            if (!json_doc.isNull() && json_err.error == QJsonParseError::NoError) // 检查数据结构
            {
                QJsonObject json_obj = json_doc.object();

                DataTableInfo info;
                EnchDataList enchs;
                QSet<Group> groups;

                if (json_obj.contains("type")) // 包含 type 键
                {
                    if (json_obj.value("type").toString("null") != "custom_table") // type 的值须是 custom_table
                        return false;
                }
                else
                {
                    DEBUG("FileOperator", "A json data table must have a 'tpye' key", "warning");
                    return false;
                }

                if (json_obj.contains("format_version")) // 包含 format_version 键
                {
                    if (json_obj.value("format_version").toInt(0) != FILEVERSION) // format_version 的值须是当前文件版本的值
                        return false;
                }
                else
                {
                    DEBUG("FileOperator", "A json data table must have a 'format_version' key", "warning");
                    return false;
                }

                if (json_obj.contains("content")) // 包含 content 键
                {
                    QJsonObject obj_content = json_obj.value("content").toObject();
                    if (obj_content.isEmpty()) // 排除空值
                    {
                        DEBUG("FileOperator", "Occurred an empty table", "warning");
                        return false;
                    }

                    if (obj_content.contains("enchantments")) // 包含 enchantments 键
                    {
                        QJsonArray obj_enchantments = obj_content.value("enchantments").toArray();
                        if (obj_enchantments.isEmpty()) // 排除空值
                        {
                            DEBUG("FileOperator", "Key 'enchantments' shouldn't be empty", "warning");
                            return false;
                        }

                        // 载入 table 的名片信息
                        info.name = f_info.filePath();
                        if (json_obj.contains("description"))
                            info.description = json_obj.value("description").toString();
                        if (json_obj.contains("table_version"))
                            info.table_version = json_obj.value("table_version").toString();

                        if (!m) // 扫描模式下停止读取，直接返回
                        {
                            table_list.append(info);
                            DEBUG("FileOperator", file_name + " checked in successfully", "info");
                            return true;
                        }

                        for (int i = 0; i < obj_enchantments.size(); i++) // 遍历 enchantments 容器
                        {
                            QJsonObject obj = obj_enchantments.at(i).toObject();
                            if ((obj.contains("name") && obj.contains("max_level") && obj.contains("poor_max_level") && obj.contains("book_multiplier") && obj.contains("item_multiplier") && obj.contains("editions") && obj.contains("specials") && obj.contains("conflictions"))) // 检查有效性
                            {
                                EnchData ench;

                                ench.name = obj.value("name").toString();
                                ench.max_lvl = obj.value("max_level").toInt();
                                ench.poor_max_lvl = obj.value("poor_max_level").toInt();
                                ench.book_multiplier = obj.value("book_multiplier").toInt();
                                ench.item_multiplier = obj.value("item_multiplier").toInt();

                                QJsonArray editions = obj.value("editions").toArray();
                                for (int j = 0; j < editions.size(); j++)
                                    ench.editions.insert(StringToMCE(editions.at(j).toString()));

                                QJsonArray specials = obj.value("specials").toArray();
                                for (int j = 0; j < specials.size(); j++)
                                    ench.specials.insert(StringToSpecialMethod(specials.at(j).toString()));

                                QJsonArray conflictions = obj.value("conflictions").toArray();
                                for (int j = 0; j < conflictions.size(); j++)
                                    ench.conflictions.insert(conflictions.at(j).toString());

                                if (obj.contains("groups")) // 读取分组
                                {
                                    QJsonArray groups = obj.value("groups").toArray();
                                    for (int j = 0; j < groups.size(); j++)
                                        ench.groups.insert(groups.at(j).toString());
                                }

                                if (ench.isValid() && !enchs.contains(ench)) // 检查数据
                                    enchs.append(ench);
                            }
                        }
                    }
                    else
                    {
                        DEBUG("FileOperator", "Custom table needs an 'enchantments' array", "warning");
                        return false;
                    }

                    if (obj_content.contains("groups")) // 若包含 groups 键就读取
                    {
                        QJsonArray obj_groups = obj_content.value("groups").toArray();
                        if (obj_groups.isEmpty()) // 排除空值
                            return false;

                        for (int i = 0; i < obj_groups.size(); i++) // 加载组名和图标
                        {
                            QJsonObject obj = obj_groups.at(i).toObject();
                            if (obj.contains("name"))
                            {
                                Group group;
                                group.name = obj.value("name").toString();
                                if (obj.contains("max_durability"))
                                    group.max_durability = obj.value("max_durability").toInt();
                                if (obj.contains("icon"))
                                    group.icon_path = obj.value("icon").toString();
                                groups.insert(group);
                            }
                            else
                                continue;
                        }

                        foreach (auto &e, enchs) // 整理组魔咒列表
                        {
                            foreach (auto &g, e.groups)
                            {
                                foreach (auto &it, groups)
                                {
                                    if (it.name == g)
                                        const_cast<Group &>(it).enchantments.insert(e);
                                }
                            }
                        }
                    }
                }
                else
                {
                    DEBUG("FileOperator", "A json data table must have a 'content' object", "warning");
                    return false;
                }

                current_table = info;
                global_u_ench_table = enchs;
                global_groups = QList<Group>(groups.begin(), groups.end());
            }
            else
            {
                DEBUG("FileOperator", json_err.errorString(), "warning");
                return false;
            }
        }
        else if (f_info.suffix().toLower() == "csv") // CSV Fromat
        {
            //            DEBUG("FileOperator", "CSV Table", "info");
            QStringList raw = QString(f.readAll()).trimmed().split('\n', Qt::SkipEmptyParts); // 读取文件全部数据并按行切分
            f.close();                                                                        // 关闭文件

            if (raw.isEmpty())
            {
                DEBUG("FileOperator", "Occurred an empty file", "warning");
                return false;
            }

            DataTableInfo info;
            EnchDataList enchs;
            QSet<Group> groups;

            QString fver_key = "file_version=";
            QString head = raw.at(0);
            head = head.remove(0, 2).trimmed();
            if (!raw.at(0).startsWith("#?") || !head.startsWith(fver_key))
            {
                DEBUG("FileOperator", "A csv data table must contain file version infomation with the start", "warning");
                return false;
            }
            head.remove(0, fver_key.size());
            QStringList first_row = head.split(',', Qt::SkipEmptyParts);
            if (first_row.isEmpty() || first_row.at(0).toInt() != FILEVERSION)
            {
                DEBUG("FileOperator", "The file version is not suitable for current version", "warning");
                return false;
            }

            info.name = f_info.filePath();
            if (!m)
            {
                table_list.append(info);
                DEBUG("FileOperator", file_name + " checked in successfully", "info");
                return true;
            }

            foreach (auto tm, raw)
            {
                QStringList data;

                tm = tm.trimmed();
                if (tm.at(0) != TEXT_NOTE_SYMBOL && tm.at(0) != ',')
                    data = tm.split(',', Qt::SkipEmptyParts);
                for (int i = 0; i < data.size(); i++)
                    data[i] = data.at(i).trimmed();
                if (data.isEmpty())
                    continue;

                if (data.at(0) == "Enchantment")
                {
                    if (data.size() < 6)
                        continue;

                    EnchData ench;
                    ench.name = data.at(1);
                    ench.max_lvl = data.at(2).toInt();
                    ench.poor_max_lvl = data.at(3).toInt();
                    ench.book_multiplier = data.at(4).toInt();
                    ench.item_multiplier = data.at(5).toInt();

                    if (6 < data.size())
                    {
                        QStringList editions = data.at(6).split(';', Qt::SkipEmptyParts);
                        for (int i = 0; i < editions.size(); i++)
                            ench.editions.insert(StringToMCE(editions.at(i)));
                    }
                    if (7 < data.size())
                    {
                        QStringList specials = data.at(7).split(';', Qt::SkipEmptyParts);
                        for (int i = 0; i < specials.size(); i++)
                            ench.specials.insert(StringToSpecialMethod(specials.at(i)));
                    }

                    if (8 < data.size())
                    {
                        QStringList conflictions = data.at(8).split(';', Qt::SkipEmptyParts);
                        ench.conflictions = QSet<QString>(conflictions.begin(), conflictions.end());
                    }
                    if (9 < data.size())
                    {
                        QStringList groups = data.at(9).split(';', Qt::SkipEmptyParts);
                        ench.groups = QSet<QString>(groups.begin(), groups.end());
                    }

                    if (ench.isValid() && !enchs.contains(ench))
                        enchs.append(ench);
                }
                else if (data.at(0) == "Group")
                {
                    if (data.size() < 2)
                        continue;

                    Group group;
                    group.name = data.at(1);
                    if (2 < data.size())
                        group.max_durability = data.at(2).toInt();
                    if (3 < data.size())
                        group.icon_path = data.at(3);

                    groups.insert(group);
                }
            }

            foreach (auto &e, enchs)
            {
                foreach (auto &g, e.groups)
                {
                    foreach (auto &it, groups)
                    {
                        if (it.name == g)
                            const_cast<Group &>(it).enchantments.insert(e);
                    }
                }
            }

            current_table = info;
            global_u_ench_table = enchs;
            global_groups = QList<Group>(groups.begin(), groups.end());
        }

        global_p_ench_table = convertEnchTable(global_u_ench_table);
        DEBUG("FileOperator", "Table data loaded", "info");
        return true;
    }
    else
    {
        DEBUG("FileOperator", "Unable to open " + file_name, "error");
        return false;
    }
}

bool FileOperator::saveTableData(QString file_name)
{
    DEBUG("FileOperator", "Saving table data...", "info");
    QFile f(file_name);

    if (f.open(QIODeviceBase::WriteOnly))
    {
        QFileInfo f_info(f);
        if (f_info.suffix().toLower() == "json")
        {
            QJsonObject root;
            root.insert("description", current_table.description);
            root.insert("table_version", current_table.table_version);
            root.insert("type", "custom_table");
            root.insert("format_version", 5);
            QJsonObject content;

            QJsonArray enchantments;
            foreach (auto &ench, global_u_ench_table)
            {
                QJsonObject e;
                e.insert("name", ench.name);
                e.insert("max_level", ench.max_lvl);
                e.insert("poor_max_level", ench.poor_max_lvl);
                e.insert("book_multiplier", ench.book_multiplier);
                e.insert("item_multiplier", ench.item_multiplier);

                QJsonArray editions;
                for (const auto &arr : ench.editions)
                    editions.append(MCEToString(arr));
                e.insert("editions", editions);

                QJsonArray specials;
                for (const auto &arr : ench.specials)
                    specials.append(SpecialMethodToString(arr));
                e.insert("specials", specials);

                QJsonArray conflictions;
                for (const auto &arr : ench.conflictions)
                    conflictions.append(arr);
                e.insert("conflictions", conflictions);

                QJsonArray groups;
                for (const auto &arr : ench.groups)
                    groups.append(arr);
                e.insert("groups", groups);

                enchantments.append(e);
            }

            QJsonArray groups;
            foreach (auto &group, global_groups)
            {
                QJsonObject g;
                g.insert("name", group.name);
                g.insert("icon", group.icon_path);
                groups.append(g);
            }

            content.insert("enchantments", enchantments);
            content.insert("groups", groups);
            root.insert("content", content);

            QJsonDocument json_doc(root);
            f.write(json_doc.toJson(QJsonDocument::Indented));
        }
        else if (f_info.suffix().toLower() == "csv")
        {
            QString data;
            data += QString("#?file_version=") + QString::number(FILEVERSION) + '\n';

            data += QString("# ") + tr("<数据类型>,<组名>,[组图标]") + '\n';
            foreach (auto &g, global_groups)
            {
                data += QString("Group,") + g.name + ',' + g.icon_path + '\n';
            }

            data += QString("# ") + tr("<数据类型>,<魔咒名称>,<最大等级>,<Poor最大等级>,<附魔书乘数>,<物品乘数>,<MC编译版本>,[特殊功能],[冲突的魔咒],[分组]") + '\n';
            foreach (auto &e, global_u_ench_table)
            {
                data += QString("Enchantment,") + e.name + ',' + QString::number(e.max_lvl) + ',' + QString::number(e.poor_max_lvl) + ',' + QString::number(e.book_multiplier) + ',' + QString::number(e.item_multiplier) + ',';
                for (const auto &arr : e.editions)
                    data += MCEToString(arr) + ';';
                data += ',';
                for (const auto &arr : e.specials)
                    data += SpecialMethodToString(arr) + ';';
                data += ',';
                for (const auto &arr : e.conflictions)
                    data += arr + ';';
                data += ',';
                for (const auto &arr : e.groups)
                    data += arr + ';';
                data += '\n';
            }

            f.write(FILEHEAD_CSV);
            f.write(data.toUtf8());
        }

        f.close();
        DEBUG("FileOperator", "Table data saved", "info");
        return true;
    }
    else
    {
        DEBUG("FileOperator", "Unable to open " + file_name, "error");
        return false;
    }
}

bool FileOperator::loadArchivePoint(QString file_name)
{
    QFile f(file_name);
    QSettings settings(file_name, QSettings::IniFormat);
    if (f.open(QIODevice::ReadOnly))
    {
        return true;
    }
    else
        return false;
}

bool FileOperator::saveArchivePoint(QString file_name)
{
    QFile f(file_name);

    return true;
}

bool FileOperator::saveResult(Summary summary, QList<FlowStep> flow, QString file_name, int mode)
{
    DEBUG("FileOperator", "Saving result...", "info");
    QFile f(file_name);
    if (!f.open(QIODevice::WriteOnly))
    {
        DEBUG("FileOperator", "Unable to open " + file_name, "error");
        return false;
    }

    QString data;

    switch (mode)
    {
    case 0: // 纯文本文件 格式一
        data = tr("[BESQ Flow Output] ") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss\n");
        data += tr("[BESQ Version] ") + VERSION_NAME + " (" + QString::number(VERSION_ID) + ")\n";
        data += tr("[Configuration] ") + MCEToString(summary.edition) + tr(" Edition, ") + "Normal mode, " + "Algorithm 1" + "\n";
        data += tr("[Brief]") + '\n' + tr("Step(s): ") + QString::number(summary.step_count) + '\n' + tr("Max Level cost: ") + QString::number(summary.max_level_cost) + '\n' + tr("Total level cost: ") + QString::number(summary.total_level_cost) + '\n' + tr("Total point cost: ") + QString::number(summary.total_point_cost) + '\n';
        data += tr("[Flow]") + '\n';

        foreach (auto &step, flow)
        {
            int n = 1;

            QString name_a = "Unknown";
            if (step.a.type == ItemType::Book)
                name_a = "Enchanted Book";
            else if (step.a.type == ItemType::Weapon)
                name_a = "Weapon";
            else if (step.a.type == ItemType::Stuff)
                name_a = "Stuff";

            QString name_b = "Unknown";
            if (step.b.type == ItemType::Book)
                name_b = "Enchanted Book";
            else if (step.b.type == ItemType::Weapon)
                name_a = "Weapon";
            else if (step.b.type == ItemType::Stuff)
                name_b = "Stuff";

            data += "[Step Start] - " + QString::number(n) + "\n";
            data += QString("[Item A] ") + name_a + "\n";
            foreach (auto &e, step.a.enchs)
                data += QString("       - ") + e.name + " " + intToLevelText(e.lvl) + "\n";
            data += QString("[Item B] ") + name_b + "\n";
            foreach (auto &e, step.b.enchs)
                data += QString("       - ") + e.name + " " + intToLevelText(e.lvl) + "\n";
            data += QString("[L Cost] ") + QString::number(step.level_cost) + "\n";
            data += QString("[P Cost] ") + QString::number(step.point_cost) + "\n";
            if (step.name_changing)
                data += QString("[Special] ") + "Change the name here!\n";
            data += QString("[Step End] \n\n");

            n++;
        }
        break;
    default:
        break;
    }

    f.close();
    DEBUG("FileOperator", "The result saved", "info");
    return true;
}
