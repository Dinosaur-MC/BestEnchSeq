#ifndef AWINDOW_H
#define AWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>

#include "basicoperator.h"
#include "settings.h"


namespace Ui {
    class AWindow;
}

class AWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AWindow(QWidget *parent = nullptr);
    ~AWindow();

    void initialize();    // 初始化
    void refreshPage(int page = 0);    // 刷新页面列表

    void initStatusBar();    // 设置状态栏
    void onFirstLaunch();   // 首次启动时展示相关信息
    void onUpdated();   // 软件更新后展示相关信息
    void restart(); // 重启

private:
    // UI
    Ui::AWindow *ui;

    // 数据源
    QVector<Option> opt;
    Config cfg;
    QVector<raw_Weapon> raw_weapon_table;
    QVector<Weapon> weapon_table;
    QVector<raw_EnchPlus> raw_enchantment_table;
    QVector<EnchPlus> enchantment_table;

    // 工作模式
    MCE mce;
    PFADDN addn;
    int pfaddn;
    ICM icm;
    ALGM alg;

    // 工作环境
    FileOperator file_opr;
    Anvil *anv;
    EnchFilter *e_filter;
    Transformer *transf;

    // 工作数据
    QLabel *lb_update;
    QString statusbar_text;


    Weapon weapon;
    raw_Weapon raw_weapon;

    Item current_item;
    Item aim_item;

    Summary summary;
    QVector<FlowStep> flow;

protected:
    // 按键事件
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:

};


PFADDN toPFADDN(bool n[3]);

#endif // AWINDOW_H
