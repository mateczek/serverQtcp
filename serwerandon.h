#ifndef SERWERANDON_H
#define SERWERANDON_H

#include <QObject>
#include<QTcpServer>
#include<QtSql>
#include<QTcpSocket>
struct rekordDB{
    int id_adtemp_val_1;
    QString arr_route_val_2;
    //QString trailer_id;
    //QDateTime arr_date;
    //QString real_time;
    QDateTime unl_date_val_6;
    //QString status;
    //QString  	notes ;
    QDateTime  	dep_date_val_9 ;
    //QString  	dep_route ;
    //QString unload_time;
    //QString  	load_time ;
    QString gate_val_13 ;
    //QDateTime date_wjazd ;
    //QDateTime  	date_rr ;
    //QDateTime  	date_zz ;
    //QDateTime date_wyjazd;
    int  	enabled_val_18 ;
    //QString	submittedby;

};

class serwerAndon : public QObject
{
    Q_OBJECT
    QTcpServer *serwer;
    QSqlDatabase baza;
    rekordDB recorddb;
    QSet<QTcpSocket*> gateA;
    void timerEvent(QTimerEvent *event);
public:
    explicit serwerAndon(QObject *parent = nullptr);
    ~serwerAndon();
    void startSerwer();
signals:
    void quit();
private slots:
    void newClient();
    void dataRecive();
    void clientDisconnect();

public slots:
};

#endif // SERWERANDON_H
