#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>

namespace Ui {
class Graphics;
}

class Graphics : public QWidget
{
    Q_OBJECT
public:
    explicit Graphics(QWidget *parent = nullptr);
    ~Graphics();

    int run();

private:
    Ui::Graphics *ui;

signals:

};

#endif // GRAPHICS_H
