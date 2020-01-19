#ifndef SIMPLEOBJECT_H
#define SIMPLEOBJECT_H

#include <QObject>


class SimpleObject : public QObject
{
    Q_OBJECT

public:
    explicit SimpleObject();
    ~SimpleObject();

public:
    void run();

Q_SIGNALS:
    void lightChanged(int value);
};

#endif // SIMPLEOBJECT_H
