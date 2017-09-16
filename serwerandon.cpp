#include "serwerandon.h"
#include <QSqlQuery>
#include<QDataStream>
#include<QDebug>
void serwerAndon::timerEvent(QTimerEvent *event)
{
    QSqlQuery query("SELECT * FROM ad_db WHERE enabled=1 AND gate='A' ORDER BY unl_date ASC LIMIT 1");
    query.exec();
    QByteArray frame;
    QDataStream out(&frame,QIODevice::WriteOnly);
    if(query.first()){
        int id=query.value(0).toInt();
        QString arr_route=query.value(1).toString();
        QDateTime unl_date=query.value(5).toDateTime();
        QDateTime dep_date=query.value(8).toDateTime();
        QString gate=query.value(12).toString();
        int enabled=query.value(17).toInt();
        out<<(qint16)(0)<<id<<arr_route<<unl_date<<dep_date<<gate<<enabled;
        out.device()->seek(0);
        out << (quint16)(frame.size() - 2);
    }else{
        out<<(qint16)(-127);
    }



    for(QTcpSocket* cli:gateA){
        cli->write(frame);
    }
}

serwerAndon::serwerAndon(QObject *parent) : QObject(parent)
{
    serwer=new QTcpServer();
    recorddb.id_adtemp_val_1=-1;
}
serwerAndon::~serwerAndon()
{
    delete serwer;

}

void serwerAndon::startSerwer()
{
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
            startTimer(1000);
        }
        else{
            statServer=false;
            qDebug()<<"problem z bazą danych";
        }
    }else{
        statServer=false;
        qDebug()<<"serwer problem - zajęty port";
    }
    if(!statServer) emit quit();
}

void serwerAndon::newClient()
{
    QTcpSocket *client=serwer->nextPendingConnection();
    connect(client,SIGNAL(readyRead()),this,SLOT(dataRecive()));
    connect(client,SIGNAL(disconnected()),this, SLOT(clientDisconnect()));
}

void serwerAndon::dataRecive()
{
    QTcpSocket*client=qobject_cast<QTcpSocket*>(sender());
    QByteArray frame=client->readLine();
    qDebug()<<frame;
    if (frame=="gateA\r\n"){
        gateA.insert(client);

        qDebug()<<"klient przypisany; liczba klientów = "<<gateA.size();
    }
}

void serwerAndon::clientDisconnect()
{
    QTcpSocket*client=qobject_cast<QTcpSocket*>(sender());
    gateA.remove(client);
    client->deleteLater();
    qDebug()<<"klient rozłączony; liczba klientów= "<<gateA.size();

}
