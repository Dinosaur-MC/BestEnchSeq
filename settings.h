#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "datamanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Settings;}
QT_END_NAMESPACE

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
    int n;  //counter for opening developer mode

signals:
    void refresh();
};

#endif // SETTINGS_H
