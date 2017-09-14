#include "serwerandon.h"
#include<QDebug>
serwerAndon::serwerAndon(QObject *parent) : QObject(parent)
{
    serwer=new QTcpServer();
    bool statServer=true;
    if(serwer->listen(QHostAddress::Any,3333)){
        connect(serwer,SIGNAL(newConnection()),this,SLOT(newClient()));
        baza=QSqlDatabase::addDatabase("QMYSQL");
        baza.setHostName("localhost");
        baza.setDatabaseName("mydb100");
        baza.setUserName("root");
        baza.setPassword("root");
        if (baza.open()){
            qDebug()<<"serwer Pracuje...............";
        }
        else{
            statServer=false;
            qDebug()<<"problem z bazą danych";
        }
    }else{
        statServer=false;
        qDebug()<<"serwer problem - zajęty port";
    }



}

void serwerAndon::newClient()
{
    QTcpSocket *client=serwer->nextPendingConnection();
    clientsList.append(client);
    qDebug()<<" nowy klijent podłączony; liczba klijentów= "<<clientsList.size();
    connect(client,SIGNAL(readyRead()),this,SLOT(dataRecive()));
    connect(client,SIGNAL(disconnected()),this, SLOT(clientDisconnect()));
}

void serwerAndon::dataRecive()
{
    QTcpSocket*client=qobject_cast<QTcpSocket*>(sender());
    qDebug()<<client->readAll();
    QByteArray response{"dzieki za info"};
    client->write(response);
}

void serwerAndon::clientDisconnect()
{
    QTcpSocket*client=qobject_cast<QTcpSocket*>(sender());
    clientsList.removeOne(client);
    client->deleteLater();
    qDebug()<<" klijent rozłączony; liczba klijentów= "<<clientsList.size();

}
