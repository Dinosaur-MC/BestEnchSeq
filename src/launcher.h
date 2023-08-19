#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QTranslator>

enum class LaunchMode {
    None = 0x00,
    ConsoleMode = 0x01,
    GraphicsMode = 0x02,
    DebugMode = 0x10,
    ReadOnlyMode = 0x20,
    SafeMode = 0x40
};

class Launcher : public QObject
{
    Q_OBJECT
public:
    Launcher() = default;
    ~Launcher();
    Launcher(const Launcher &) = delete;

    int launch(LaunchMode mode);

private:

signals:
    void launched();
    void stopped(int code);
};

#endif // LAUNCHER_H
