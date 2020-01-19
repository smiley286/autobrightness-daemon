#include "daemon.h"


#include <QThread>


Daemon::Daemon()
{
    daemonStart();
}

Daemon::~Daemon()
{
}

void Daemon::daemonStart(){

    thread = new QThread;

    objThread = new SimpleObject;
    connect(objThread, &SimpleObject::lightChanged, this, [=](int value){
        QString cmd = QString("touch /home/kylin/receive");
        QByteArray ba;
        ba = cmd.toUtf8();
        system(ba.data());
    });


    objThread->moveToThread(thread);

    connect(thread, &QThread::finished, objThread, &SimpleObject::deleteLater);
    connect(thread, &QThread::started, objThread, &SimpleObject::run);

    thread->start();

}
