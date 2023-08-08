#ifndef ALGGREEDY_H
#define ALGGREEDY_H

#include <QObject>
#include "core/algorithm.h"

class AlgGreedy : public Algorithm
{
    Q_OBJECT
public:
    explicit AlgGreedy(QObject *parent = nullptr);

private:
    virtual void run() override;

signals:

};

#endif // ALGGREEDY_H
