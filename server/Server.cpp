//
// Created by Vetas Ramvet on 29.10.2017.
//

#include "Server.hpp"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    qsrand(static_cast<uint>(time(nullptr)));
    int port = qrand() % (65536 - 6000) + 1024; //Магия с портом, на котором сервер будет слушать

    start(QHostAddress::Any, quint16(port));
}

void Server::start(const QHostAddress &address, quint16 port)
{
    connect(this, &Server::newConnection, this, &Server::slotNewConnection);
    connect(this, &Server::acceptError, this, &Server::slotNewConnection);
    if (!this->listen(address, port))
    {
        qDebug() << "Server is nor working!";
    }
    else
    {
        qDebug() << "Server is started \n"
                 << "Ip-address : " << this->serverAddress().toString()
                 << "\n Port : " << this->serverPort();
    }
}

void Server::incomingConnection(qintptr socketDescriptor) //Можно модифицировать сокет (дескрипторы, IOMode, вот это все)
{
    emit QTcpServer::incomingConnection(socketDescriptor);
}

void Server::sendMessageToGroup() //Пока ничего не делает, т.к. нет сериализации групп и вообще всего этого
{
    //QSet<User> users;
    //Выбираем всех пользователей, принадлежащих группе
    //Отсылаем им сообщение
}

void Server::slotNewConnection()
{
    QTcpSocket *client = nextPendingConnection();
    clients.insert(client);
    client->write("Hello, I'am a Chapp server\n");

    qDebug() << "New client : " << client->peerAddress().toString();

    connect(client, &QTcpSocket::readyRead, this, &Server::slotServerRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::slotClientDisconnected);
    sendMessageToGroup();
}

void Server::slotServerRead() //Читаем информацию из сокета
{
    auto *client = (QTcpSocket*)sender();

    while(client->bytesAvailable() > 0)
        // Выбрав формат передачи данных мы будем ждать
        // определенное кол-во байт
        // и можно будет принимать QByteArray целиком
    {
        QString readString = client->readAll();
        QByteArray array;
        array.append(readString);
        std::cout << array.toStdString(); //Выводим в лог
        std::string str = array.toStdString();
        if (str == "end\r\n")
            client->close();

        client->write("Answer\n");
    }
}

void Server::slotClientDisconnected()
{
    auto *client = (QTcpSocket*)sender();
    qDebug() << "Someone has disconnected : " << client->peerAddress();
    client->close();
    if (clients.contains(client))
    {
        clients.erase(clients.find(client));
    }
}

void Server::stop()
{
    foreach(QTcpSocket* val, clients)
        {
            val->close();
        }
    close();
}