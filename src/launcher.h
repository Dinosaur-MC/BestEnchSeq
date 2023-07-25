#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QApplication>
#include "core/core.h"
#include "core/fileoperator.h"
#include "core/calculator.h"

class Launcher : public QObject
{
    Q_OBJECT
public:
    explicit Launcher(Core *c, QCoreApplication *a);
    explicit Launcher(Core *c, QApplication *a);

    int launch();
    bool initialize();

private:
    MDebug mdb;

    Core *core;
    QCoreApplication *capp;
    QApplication *app;
    int mode;

    FileOperator fopr;
    Calculator calc;

signals:

};

#endif // LAUNCHER_H
