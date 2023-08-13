#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QTranslator>
#include "core/tablemanager.h"
#include "updatechecker.h"
#include "ui/console.h"
#include "ui/graphics.h"

class Launcher : public QObject {
    Q_OBJECT
public:
    Launcher() = default;
    Launcher(const Launcher &) = delete;
    ~Launcher();

    int launch(int mode);
    void stop();
    int restart();

private:
    QTranslator translator;
    UpdateChecker *update;
    TableManager table_mgr;

    Console *console;
    Graphics *graphics;

signals:
    void launched();
    void stopped(int code);
};

#endif // LAUNCHER_H
