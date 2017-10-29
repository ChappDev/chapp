//
// Created by Vetas Ramvet on 29.10.2017.
//

#include "Server.hpp"
#include <QDebug>

Server::Server(QObject *parent)
{
    tcpServer = new QTcpServer(this);

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::slotNewConnection);

    qsrand(time(nullptr));
    int port = qrand() % (65536-6000) + 1000;

    if (!tcpServer->listen(QHostAddress::Any, quint16(port)))
    {
        qDebug() << "Server is not working!";
    }
    else
    {
        qDebug() << "Server is started \n"
                 << "Ip-address : " << tcpServer->serverAddress().toString()
                 << "\n Port : " << tcpServer->serverPort();
    }
}

void Server::slotNewConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    qDebug() << "New connection!";

    tcpSocket->write("Hello");

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Server::slotServerRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Server::slotClientDisconnected);
}

void Server::slotServerRead()
{
    //TODO: Реализовать сериализацию в базу (где база?)
    //Возникли проблемы с определением отправителя сообщения.
    //QTcpSocket *sender = qobject_cast<QTcpSocket*>(sender());
    /*
    QTcpSocket *sender = qobject_cast<QTcpSocket*>(sender());
    while(sender->bytesAvailable() > 0)
    {
        QString readString = sender->readAll();
        QByteArray array;
        array.append(readString);
        std::cout << array.toStdString();
        sender->write("I'am answering you");
    }
     */
}

void Server::slotClientDisconnected()
{
    qDebug() << "Someone has disconnected";

    tcpSocket->close();
}
