#ifndef ENCHLIST_H
#define ENCHLIST_H

#include <QListWidget>

class EnchList : public QListWidget
{
    Q_OBJECT
public:
    explicit EnchList(QWidget *parent = nullptr);

    bool setWeapon(QString name);
    QString Weapon();
    int enchCount();
    void refresh();
    void uploadList();
    void checkRepulsion();

private:
    int current_weapon_index;

signals:

};

#endif // ENCHLIST_H
