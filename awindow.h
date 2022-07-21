#ifndef AWINDOW_H
#define AWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

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

    void loadInternalData();    // 加载内置数据
    void initialize();    // 初始化
    void refreshPage(int page = 0);    // 刷新页面列表

    void initStatusBar();    // 设置状态栏
    void onFirstLaunch();   // 首次启动时展示相关信息
    void onUpdated();   // 软件更新后展示相关信息
    void restart(); // 重启

private:
    Ui::AWindow *ui;


protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:

};

#endif // AWINDOW_H
