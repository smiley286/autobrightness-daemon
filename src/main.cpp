#include "daemon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Daemon w;
    w.daemonStart();
//    w.show();

    return a.exec();
}
