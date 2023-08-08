#ifndef ALGENUMERATE_H
#define ALGENUMERATE_H

#include <QObject>
#include "core/algorithm.h"

class AlgEnumerate : public Algorithm
{
    Q_OBJECT
public:
    explicit AlgEnumerate(QObject *parent = nullptr);

private:
    virtual void run() override;

signals:

};

#endif // ALGENUMERATE_H
