#include "graphics.h"
#include "mainwindow.h"
#include <QApplication>

Graphics::Graphics(QObject *parent)
    : QObject{parent}
{

}

int Graphics::run()
{
    qDebug() << "Graphics running";

    MainWindow w;
    w.show();
    int ret = QApplication::exec();

    qDebug() << "Graphics exited";
    return ret;
}

int Graphics::exit(int code)
{
    return code;
}

