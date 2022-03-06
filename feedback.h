#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QObject>

class Feedback : public QObject
{
    Q_OBJECT
public:
    explicit Feedback(QObject *parent = nullptr);

signals:

};

#endif // FEEDBACK_H
