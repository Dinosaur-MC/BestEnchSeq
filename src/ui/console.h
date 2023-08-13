#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>

class Console : public QObject
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = nullptr);

    int run();
    int exit(int code);

private:

signals:

};

#endif // CONSOLE_H
