#include <QCoreApplication>
#include "serwerandon.h"
#include<QObject>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    serwerAndon s;
    QObject::connect(&s,SIGNAL(quit()),qApp,SLOT(quit()),Qt::QueuedConnection);
    s.startSerwer();
    return a.exec();
}
