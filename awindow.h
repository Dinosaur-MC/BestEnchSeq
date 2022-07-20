#ifndef AWINDOW_H
#define AWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

namespace Ui {
    class AWindow;
}

class AWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AWindow(QWidget *parent = nullptr);
    ~AWindow();


private:
    Ui::AWindow *ui;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:

};

#endif // AWINDOW_H
