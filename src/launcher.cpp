#include "launcher.h"
#include "mainwindow.h"
#include "windows.h"
#include "iostream"
#include <QDialog>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QFont>
#include "checkupdate.h"

using namespace std;

Launcher::Launcher(Core *c, QCoreApplication *a)
    : mdb("Launcher"), fopr(c), calc(c)
{
    core = c;
    capp = a;
    mode = 0;
}

Launcher::Launcher(Core *c, QApplication *a)
    : mdb("Launcher"), fopr(c), calc(c)
{
    core = c;
    app = a;
    mode = 1;
}

int Launcher::launch()
{
    if(mode == 0)   // 控制台模式
    {
        AllocConsole(); // 创建控制台
        system("chcp 65001");

        // 重定向数据流
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);

        if(!initialize())
            return 1;
        cout << "- CONSOLE MODE -" << endl;

        int ret = 0;
        do {

        } while(ret > 0);   // 重启循环

        system("pause");
        fclose(stdout);
        fclose(stdin);
        FreeConsole();
        return 0;
    }

    else if(mode == 1)  // GUI模式
    {
        if(!initialize())
            return 1;
        qDebug() << "- GUI MODE -";

        int ret = 0;
        do {
            // 创建并启动主窗口
            MainWindow w;
            w.show();
            ret = app->exec();
        } while(ret == RESTART_CODE);   // 重启循环

        return ret;
    }

    else
    {
        return -1;
    }
}

bool Launcher::initialize()
{
    QFile fp(FILE_CONFIG);  // 首次启动声明（配置文件不存在时启用）
    if(!fp.exists())
    {
        if(mode == 0)
        {
            cout << "================================================" << endl;
            cout << PROGRAM_NAME_CN << " " << VERSION << "(" << VERSION_ID << ")" << endl;
            cout << PROGRAM_NAME_EN << "(" << PROGRAM_NAME_ABBR << ")" << endl;
            cout << "================================================" << endl;
            cout << "Github: " << WEBSITE << endl;
            cout << STATEMENT << endl;
            cout << "================================================" << endl;
            cout << "确认知晓以上情况并开始使用本计算器，请输入'Y'；退出程序，请输入'N'或其它字符" << endl;
            cout << "> ";
            char c;
            cin >> c;
            if(c != 'Y')
                return false;
        }
        else
        {
            QDialog *dialog = new QDialog();
            dialog->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
            dialog->setWindowTitle("欢迎使用 Welcome to use");
            QVBoxLayout *layout = new QVBoxLayout(dialog);
            layout->setSizeConstraint(QLayout::SetFixedSize);

            QFont ft;
            ft.setPointSize(20);
            QLabel *title = new QLabel(dialog);
            title->setFont(ft);
            title->setAlignment(Qt::AlignCenter);
            title->setText(QString(PROGRAM_NAME_CN) + " " + VERSION + "(" + QString::number(VERSION_ID) + ")" + "\n" + PROGRAM_NAME_EN + "(" + PROGRAM_NAME_ABBR + ")");

            QFont ft2;
            ft2.setPointSize(12);
            QLabel *text = new QLabel(dialog);
            text->setFont(ft2);
            text->setWordWrap(true);
            text->setText(QString("\n【声明】") + STATEMENT + "\n\n");

            QPushButton *btnY = new QPushButton(dialog);
            btnY->setFont(ft2);
            btnY->setText("我已知晓并开始使用");
            connect(btnY, &QPushButton::clicked, dialog, &QDialog::accept);
            QPushButton *btnN = new QPushButton(dialog);
            btnN->setFont(ft2);
            btnN->setText("退出程序");
            connect(btnN, &QPushButton::clicked, dialog, &QDialog::reject);

            layout->addWidget(title);
            layout->addWidget(text);
            layout->addWidget(btnY);
            layout->addWidget(btnN);
            dialog->setLayout(layout);
            dialog->show();
            dialog->setModal(true);
            if(dialog->exec() != QDialog::Accepted)
                return false;
        }
    }

    fopr.loadConfig();  // 加载配置

    if(core->is_on_update)  // 更新完成提示
    {
        fopr.saveConfig();
        if(mode == 0)
        {
            cout << "========================================" << endl;
            cout << PROGRAM_NAME_CN << "已升级至" << VERSION << endl;
            cout << "版本ID: " << VERSION_ID << endl;
            cout << "========================================" << endl;
            cout << endl;
        }
        else
        {
            QDialog *dialog = new QDialog();
            dialog->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
            dialog->setWindowTitle("提示");
            QVBoxLayout *layout = new QVBoxLayout(dialog);
            layout->setSizeConstraint(QLayout::SetFixedSize);

            QFont ft;
            ft.setPointSize(12);
            QLabel *title = new QLabel(dialog);
            title->setFont(ft);
            title->setAlignment(Qt::AlignCenter);
            title->setText(PROGRAM_NAME_CN);

            QFont ft2;
            ft2.setPointSize(10);
            QLabel *text = new QLabel(dialog);
            text->setFont(ft2);
            text->setWordWrap(true);
            text->setText(QString("\n已升级至") + VERSION + "！\n" + "版本ID: " + QString::number(VERSION_ID));

            QPushButton *btn = new QPushButton(dialog);
            connect(btn, &QPushButton::clicked, dialog, &QDialog::accept);
            btn->setFont(ft2);
            btn->setText("好的");

            layout->addWidget(title);
            layout->addWidget(text);
            layout->addWidget(btn);
            dialog->setLayout(layout);
            dialog->show();
            dialog->setModal(true);
            dialog->exec();
        }
    }

    if(core->config.deverloper_mode)    // 开发者模式
    {

    }
    if(core->config.auto_check_update)  // 检查更新
    {
        CheckUpdate *cu = new CheckUpdate();
        cu->setUrl(QUrl(UPDATE_SITE));
        cu->start(false);
    }

    if(core->config.enable_custom_we)   // 加载自定义武器池
    {
        fopr.loadWeapons();
    }
    if(core->config.enable_custom_en)   // 加载自定义魔咒池
    {
        fopr.loadEnchantments();
    }
    if(core->config.enable_custom_we || core->config.enable_custom_en)  // 解析池数据
        fopr.updateToCore();


    // **方法试验**
    core->_methodTest();

    return true;
}

