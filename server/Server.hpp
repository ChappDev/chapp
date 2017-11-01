//
// Created by Vetas Ramvet on 29.10.2017.
//

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
class AbstractGroup;
class User;
class Message;

//TODO: немного изменить логику
class Server : public QTcpServer
{
    Q_OBJECT
public:

    explicit Server(QObject *parent = 0);

    void stop();

    void incomingConnection(qintptr socketDescriptor) override; //Необходимо перехватить входящее подключение для реализации авторизации

    void start(const QHostAddress &address, quint16 port);

    void sendMessageToGroup(); //TODO: Реализовать, учитывая БД и логику

public slots:

    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:

    QSet<QTcpSocket*> clients;
};


#endif //SERVER_SERVER_HPP
