#ifndef SERWERANDON_H
#define SERWERANDON_H

#include <QObject>
#include<QTcpServer>
#include<QtSql>
#include<QTcpSocket>
class serwerAndon : public QObject
{
    Q_OBJECT
    QTcpServer *serwer;
    QSqlDatabase baza;
    QList<QTcpSocket*> clientsList;
public:
    explicit serwerAndon(QObject *parent = nullptr);

signals:

private slots:
    void newClient();
    void dataRecive();
    void clientDisconnect();

public slots:
};

#endif // SERWERANDON_H
