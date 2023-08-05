#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QObject>

class interactor : public QObject
{
    Q_OBJECT
public:
    explicit interactor(QObject *parent = nullptr);

signals:
};

#endif // INTERACTOR_H
