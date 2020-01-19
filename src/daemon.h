#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>

#include "simpleobject.h"

class QThread;


class Daemon : public QObject
{
    Q_OBJECT

public:
    explicit Daemon();
    ~Daemon();

public:
    void daemonStart();

private:
    QThread * thread;

    SimpleObject * objThread;
};

#endif // DAEMON_H
