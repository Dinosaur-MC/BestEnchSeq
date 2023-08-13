#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QObject>

class Graphics : public QObject
{
    Q_OBJECT
public:
    explicit Graphics(QObject *parent = nullptr);

    int run();
    int exit(int code);

private:

signals:

};

#endif // GRAPHICS_H
