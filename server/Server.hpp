//
// Created by Vetas Ramvet on 29.10.2017.
//

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>


//TODO: Унаследовать от QTcpServer и немного изменить логику
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;

};


#endif //SERVER_SERVER_HPP
