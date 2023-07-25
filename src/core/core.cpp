#include "core.h"
#include <QMessageBox>

void loadInternalData(Config *config)
{
    // 程序默认设置值
    config->config_version = VERSION_ID;
    config->default_edition = MCE::Java;
    config->default_itemconfig = ICM::AllLevelEBook;
    config->default_algorithm = ALGM::AlgM_0;
    config->export_path = "";
    config->auto_save = false;
    config->auto_check_update = true;
    config->enable_custom_we = false;
    config->enable_custom_en = false;
    config->enable_reszie_window = false;
    config->deverloper_mode = false;
}

void loadInternalData(QVector<WeaponPlus> *wps)
{
    // 内置武器数据表
    wps->append({"剑", QIcon(":/icon/netherite_sword.png"), {}});
    wps->append({"镐", QIcon(":/icon/netherite_pickaxe.png"), {}});
    wps->append({"斧",QIcon(":/icon/netherite_axe.png"), {}});
    wps->append({"铲", QIcon(":/icon/netherite_shovel.png"), {}});
    wps->append({"锄", QIcon(":/icon/netherite_hoe.png"), {}});
    wps->append({"头盔", QIcon(":/icon/netherite_helmet.png"), {}});
    wps->append({"胸甲", QIcon(":/icon/netherite_chestplate.png"), {}});
    wps->append({"护腿", QIcon(":/icon/netherite_leggings.png"), {}});
    wps->append({"靴", QIcon(":/icon/netherite_boots.png"), {}});
    wps->append({"弓", QIcon(":/icon/bow.png"), {}});
    wps->append({"弩", QIcon(":/icon/crossbow_standby.png"), {}});
    wps->append({"三叉戟", QIcon(":/icon/trident.png"), {}});
    wps->append({"钓鱼竿", QIcon(":/icon/fishing_rod.png"), {}});
}

void loadInternalData(QVector<EnchPlus> *eps)
{
    // 内置魔咒数据表
    eps->append({"水下速掘", MCE::All, 1, 1, {4, 2}, {}});
    eps->append({"节肢杀手", MCE::All, 5, 4, {2, 1}, {{"锋利"},{"亡灵杀手"}}});
    eps->append({"爆炸保护", MCE::All, 4, 4, {4, 2}, {{"火焰保护"},{"弹射物保护"},{"保护"}}});
    eps->append({"引雷", MCE::All, 1, 1, {8, 4}, {{"激流"}}});
    eps->append({"绑定诅咒", MCE::All, 1, 0, {8, 4}, {}});
    eps->append({"消失诅咒", MCE::All, 1, 0, {8, 4}, {}});
    eps->append({"深海探索者", MCE::All, 3, 3, {4, 2}, {{"冰霜行者"}}});
    eps->append({"效率", MCE::All, 5, 4, {1, 1}, {}});
    eps->append({"摔落保护", MCE::All, 4, 4, {2, 1}, {}});
    eps->append({"火焰附加", MCE::All, 2, 2, {4, 2}, {}});
    eps->append({"火焰保护", MCE::All, 4, 4, {2, 1}, {{"爆炸保护"},{"弹射物保护"},{"保护"}}});
    eps->append({"火矢", MCE::All, 1, 1, {4, 2}, {}});
    eps->append({"时运", MCE::All, 3, 3, {4, 2}, {{"精准采集"}}});
    eps->append({"冰霜行者", MCE::All, 2, 0, {4, 2}, {{"深海探索者"}}});
    eps->append({"穿刺", MCE::Java, 5, 5, {4, 2}, {}});
    eps->append({"穿刺", MCE::Bedrock, 5, 5, {2, 1}, {}});
    eps->append({"无限", MCE::All, 1, 1, {8, 4}, {{"经验修补"}}});
    eps->append({"击退", MCE::All, 2, 2, {2, 1}, {}});
    eps->append({"抢夺", MCE::All, 3, 3, {4, 2}, {}});
    eps->append({"忠诚", MCE::All, 3, 3, {1, 1}, {{"激流"}}});
    eps->append({"海之眷顾", MCE::All, 3, 3, {4, 2}, {}});
    eps->append({"饵钓", MCE::All, 3, 3, {4, 2}, {}});
    eps->append({"经验修补", MCE::All, 1, 0, {4, 2}, {{"无限"}}});
    eps->append({"多重射击", MCE::All, 1, 1, {4, 2}, {{"穿透"}}});
    eps->append({"穿透", MCE::All, 4, 4, {1, 1}, {{"多重射击"}}});
    eps->append({"力量", MCE::All, 5, 4, {1, 1}, {}});
    eps->append({"弹射物保护", MCE::All, 4, 4, {2, 1}, {{"爆炸保护"},{"火焰保护"},{"保护"}}});
    eps->append({"保护", MCE::All, 4, 4, {1, 1}, {{"爆炸保护"},{"火焰保护"},{"弹射物保护"}}});
    eps->append({"冲击", MCE::All, 2, 2, {4, 2}, {}});
    eps->append({"快速装填", MCE::All, 3, 3, {2, 1}, {}});
    eps->append({"水下呼吸", MCE::All, 3, 3, {4, 2}, {}});
    eps->append({"激流", MCE::All, 3, 3, {4, 2}, {{"引雷"},{"忠诚"}}});
    eps->append({"锋利", MCE::All, 5, 4, {1, 1}, {{"节肢杀手"},{"亡灵杀手"}}});
    eps->append({"精准采集", MCE::All, 1, 1, {8, 4}, {{"时运"}}});
    eps->append({"亡灵杀手", MCE::All, 5, 4, {2, 1}, {{"节肢杀手"},{"锋利"}}});
    eps->append({"灵魂疾行", MCE::All, 3, 0, {8, 4}, {}});
    eps->append({"横扫之刃", MCE::Java, 3, 3, {4, 2}, {}});
    eps->append({"荆棘", MCE::All, 3, 3, {8, 4}, {}});
    eps->append({"耐久", MCE::All, 3, 3, {2, 1}, {}});
    eps->append({"迅捷潜行", MCE::All, 3, 0, {8, 4}, {}});
}

QString numToRoman(int num) // 魔咒等级 阿拉伯数字转罗马数字
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

long levelToPoint(int level)  // 经验等级转换为经验值
{
    long point;
    if(level <= 16)
        point = (level * level + 6 * level);
    else if(level <= 31)
        point = (2.5 * level * level - 40.5 * level + 360);
    else
        point = (4.5 * level * level - 162.5 * level + 2220);
    return point;
}


MDebug::MDebug(QString name, QObject *parent)
    : QObject{parent}
{
    // 设置消息发送者
    method_name = name;
}

QString MDebug::name()
{
    return method_name;
}

void MDebug::setName(QString name)
{
    method_name = name;
}

void MDebug::msg(MsgType type, QString str, bool l, int i)
{
    // 判断消息类型
    QString addition;
    if(type == MsgType::info)
        addition = "info";
    else if(type == MsgType::warning)
        addition = "warning";
    else if(type == MsgType::wrong)
        addition = "wrong";
    else if(type == MsgType::error)
        addition = "error";
    else
        addition = "unknow";

    // QDebug
    if(l)
    {
        qDebug() << QString("<" + method_name + ">" + "[" + addition + "][" + QString::number(i) + "] " + str).toUtf8().data();
    }
    else
    {
        qDebug() << QString("<" + method_name + ">" + "[" + addition + "] " + str).toUtf8().data();
    }

    // error级的消息将会弹窗警告并随后结束进程
    if(type == MsgType::error)
    {
        QMessageBox msgb(QMessageBox::Critical, method_name, str);
        msgb.show();
        msgb.exec();
        exit(1);
    }
}

void MDebug::msg(QString str, bool l, int i)
{
    // 消息默认以info级发送
    if(l)
    {
        qDebug() << QString("<" + method_name + ">" + "[info][" + QString::number(i) + "] " + str).toUtf8().data();
    }
    else
    {
        qDebug() << QString("<" + method_name + ">" + "[info] " + str).toUtf8().data();
    }
}

void MDebug::msg(MsgType type, QStringList strs, bool l, int i)
{
    // 判断消息类型
    QString addition;
    if(type == MsgType::info)
        addition = "info";
    else if(type == MsgType::warning)
        addition = "warning";
    else if(type == MsgType::wrong)
        addition = "wrong";
    else if(type == MsgType::error)
        addition = "wrong";
    else
        addition = "unknow";

    // QDebug
    if(l)
    {
        qDebug() << QString("<" + method_name + ">" + "[" + addition + "][" + QString::number(i) + "] ").toUtf8().data() << strs;
    }
    else
    {
        qDebug() << QString("<" + method_name + ">" + "[" + addition + "] ").toUtf8().data() << strs;
    }

    // error级的消息将会弹窗警告并随后结束进程
    if(type == MsgType::error)
    {
        // QStringList -> QString
        QString str;
        for(int i = 0; i < strs.count(); i++)
            str += strs.at(i);

        QMessageBox msgb(QMessageBox::Critical, method_name, str);
        msgb.show();
        msgb.exec();
        exit(1);
    }
}

void MDebug::msg(QStringList strs, bool l, int i)
{
    // 消息默认以info级发送
    if(l)
    {
        qDebug() << QString("<" + method_name + ">" + "[info][" + QString::number(i) + "] ").toUtf8().data() << strs;
    }
    else
    {
        qDebug() << QString("<" + method_name + ">" + "[info] ").toUtf8().data() << strs;
    }
}

MDebug StringKnife::mdb("StringKnife");
StringKnife::StringKnife() {}

QStringList StringKnife::lineSlice(QString str, char c) // 逐行分割（一维分割）
{
    if(str.isEmpty())
        return QStringList();

    QStringList strs = str.trimmed().split(c);
    QString s = strs.at(0).simplified();

    for(int i = 0; i < strs.count(); i++)
    {
        if(strs.at(i).isEmpty())
        {
            strs.takeAt(i);
            i--;
        }
        else if(strs.at(i).at(0) == TEXT_NOTE_SYMBOL || strs.at(i).isEmpty())
        {
            strs.takeAt(i);
            i--;
        }
        else
            strs[i] = strs.at(i).simplified();
    }

    // 保留文件版本信息
    if(s.startsWith("#?"))
    {
        s.remove(0, 2);
        QStringList ss = s.split('=');
        if(ss.count() == 2 && ss.at(0).trimmed() == "file_version")
        {
            strs.append(TEXT_NOTE_SYMBOL + ss.at(1).trimmed());
        }
    }

    return strs;
}

QList<QStringList> StringKnife::crossSlice(QString str, char lc, char cc, int n)    // 二维齐次分割
{
    if(str.isEmpty())
        return QList<QStringList>();

    QList<QStringList> strx;
    if(n < 2 || lc == cc)
        return strx;

    QString s = "";
    QStringList strs = lineSlice(str, lc);
    if(strs.isEmpty())
        return QList<QStringList>();
    if(strs.last().startsWith(TEXT_NOTE_SYMBOL))    // 保留文件版本信息
    {
        s = strs.takeLast().split(cc).at(0);
    }
    int ln = strs.count();

    for(int i = 0; i < ln; i++)
    {
        QStringList tm = strs.at(i).split(cc);
        if(tm.count() == n)
        {
            for(int j = 0; j < n; j++)
                tm[j] = tm[j].trimmed();
            strx.append(tm);
        }
    }

    if(!s.isEmpty())
        strx.append(QStringList() << s);
    return strx;
}

QList<QStringList> StringKnife::multicrossSlice(QString str, char lc, char cc)  // 二维非齐次分割
{
    if(str.isEmpty())
        return QList<QStringList>();

    QList<QStringList> strx;
    if(lc == cc)
        return strx;

    QString s = "";
    QStringList strs = lineSlice(str, lc);
    if(strs.isEmpty())
        return QList<QStringList>();
    if(strs.last().startsWith(TEXT_NOTE_SYMBOL))    // 保留文件版本信息
    {
        s = strs.takeLast().split(cc).at(0);
    }
    int ln = strs.count();

    for(int i = 0; i < ln; i++)
    {
        QStringList tm = strs.at(i).simplified().split(cc);
        for(int j = 0; j < tm.count(); j++)
            tm[j] = tm[j].trimmed();
        strx.append(tm);
    }

    if(!s.isEmpty())
        strx.append(QStringList() << s);
    return strx;
}


Q_GLOBAL_STATIC(Core, core);    // 全局单例类
Core::Core(QObject *parent)
    : QObject{parent},
      mdb("Core")
{
    // 初始化
    startDT = QDateTime::currentDateTime();
    mdb.msg("Start at " + startDT.toString("hh:mm:ss.zzz"));

    // 加载内置数据，设置默认值
    loadInternalData(&config);
    loadInternalData(&ench_table);
    loadInternalData(&weapon_table);
}

Core *Core::getInstance()   // 获取全局单例类指针
{
    return core;
}


#include "core/calculator.h"
// 测试函数
void Core::_methodTest()
{
    mdb.setName("_methodTest");


    // 锻造模拟
    int f = 7;

    QVector<int> pool;
    for(int i = 0; i  < pow(2, f); i++)
    {
        int n = 0;
        for(int j = i; j > 0; j/=2)
        {
            if(j%2 == 1)
                n++;
        }
        pool.append(n);
    }

    QString msg;
    msg += "{";
    for(int i = 0; i < pool.count(); i++)
    {
        if(pool.at(i) == 1)
            msg += "[" + QString::number(pool.at(i)) + "]";
        else
            msg += QString::number(pool.at(i));
        if(i < pool.count()-1)
            msg += ", ";
    }
    msg += "}";
    mdb.msg(msg);


    // 计算测试
    summary.input_item = {"剑", QIcon(), ItemType::weapon, QVector<Ench>({}), 100, 0};
    summary.output_item = {"剑", QIcon(), ItemType::weapon, QVector<Ench>({{"锋利", 5}, {"抢夺", 3}, {"耐久", 3}, {"经验修补", 1}, {"横扫之刃", 3}, {"火焰附加", 2}, {"击退", 2}}), 100, 0};

    Calculator *calc = new Calculator(this);
    calc->initialize();
    calc->setInputItem(summary.input_item);
    calc->setOutputItem(summary.output_item);

//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"锋利", 2}}), 0, 0});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"锋利", 2}}), 0, 0});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"锋利", 3}}), 0, 1});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"锋利", 3}}), 0, 1});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"抢夺", 3}, {"击退", 1}}), 0, 0});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"耐久", 3}}), 0, 1});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"经验修补", 1}}), 0, 0});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"横扫之刃", 2}}), 0, 0});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"火焰附加", 2}, {"横扫之刃", 2}, {"锋利", 3}}), 0, 2});
//    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"击退", 1}}), 0, 0});

    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"击退", 2}}), 0, 0});
    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"锋利", 5}}), 0, 0});
    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"抢夺", 3}}), 0, 0});
    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"耐久", 3}}), 0, 0});
    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"横扫之刃", 3}}), 0, 0});
    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"火焰附加", 2}}), 0, 0});
    calc->appendInPool({ID_ECB, QIcon(), ItemType::enchanted_book, QVector<Ench>({{"经验修补", 1}}), 0, 0});

    connect(calc, &QThread::finished, this, &Core::_methodTest_2);
    connect(calc, &QThread::finished, calc, &QThread::deleteLater);
    calc->start();


}

void Core::_methodTest_2()
{
    mdb.msg(QString("Summary\n") + "Step:"+QString::number(summary.step_count) + ", Time:"+QString::number(summary.time_cost) + "ms, Cost:"+QString::number(summary.level_cost)+ "L ("+QString::number(summary.point_cost)+")");
    mdb.msg("计算完成！");
}

