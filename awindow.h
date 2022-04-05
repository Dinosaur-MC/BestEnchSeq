/*
 * 【程序主窗口】
 * 程序管理
*/

#ifndef AWINDOW_H
#define AWINDOW_H

#include <QMainWindow>
#include "datamanager.h"

namespace Ui {
class AWindow;
}

class AWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AWindow(QWidget *parent = nullptr);
    ~AWindow();

    void loadInternalData();    //加载内部数据，自定义模式关闭时生效
    void init();    //初始化
    void refreshPage(int page = 0);    //刷新页面列表 (page; 0:Reflush all, 1:Reflush page 1 and clean other pages, 2:Reflush page 2, 3:Reflush Page 3)

    void setStatusBarText();    //设置状态栏
    void onFirstLaunch();   //首次启动时展示相关信息
    void onUpdated();   //软件更新后展示相关信息
    void restart(); //软件重启

private:
    Ui::AWindow *ui;
};

#endif // AWINDOW_H
