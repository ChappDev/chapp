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

/*! @brief starting the server
///
/// @param address -- an IP-address that server should listen
/// @param port -- a port that server should listen
*/
void Server::start(const QHostAddress &address, quint16 port)
{
    connect(this, &Server::newConnection, this, &Server::slotNewConnection);
    connect(this, &Server::acceptError, this, &Server::slotNewConnection);
    if (!this->listen(address, port))
    {
        qDebug() << "Server is not working!";
    }
    else
    {
        qDebug() << "Server is started \n"
                 << "Ip-address : " << this->serverAddress().toString()
                 << "\n Port : " << this->serverPort();
    }
}

/// incomingConnection -- handles incomming connections (before slotNewConnection)
///
/// @param socketDescriptor -- the socket's descriptor that should be used
void Server::incomingConnection(qintptr socketDescriptor) //Можно модифицировать сокет (дескрипторы, IOMode, вот это все)
{
    emit QTcpServer::incomingConnection(socketDescriptor);
}

/// sendMessageToGroup -- sending messages to groups
///
/// @param message -- message, obviously
/// @param address -- a group that should receive message
void Server::sendMessageToGroup() //Пока ничего не делает, т.к. нет сериализации групп и вообще всего этого
{
    //QSet<User> users;
    //Выбираем всех пользователей, принадлежащих группе
    //Отсылаем им сообщение
}

/// slotNewConnection -- handles new connections
void Server::slotNewConnection()
{
    QTcpSocket* clientSocket = nextPendingConnection();
    Client* client = new Client();
    clients.insert(clientSocket,client);
    RequestQueue* queue = client->queueOfRequests;
    queue->addCommandToQueue(RequestQueue::Cmd::initDiffieHellman);
    sendMsg(clientSocket);
    qDebug() << "New client : " << clientSocket->peerAddress().toString();

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::slotClientDisconnected);
    sendMessageToGroup();
}
void Server::sendMsg(QTcpSocket* socket) {
    RequestQueue* queue = clients[socket]->queueOfRequests;
    QByteArray* sendString = new QByteArray();
    sendString = queue->makeRequest(*sendString);
    socket->write(*sendString);
    //todo: delete this on client
    delete sendString;
}
/// slotServerRead -- reading data from the socket
void Server::slotServerRead() //Читаем информацию из сокета
{
    auto *client = (QTcpSocket*)sender();
    auto queue = clients[client]->queueOfRequests;
    while(client->bytesAvailable() > 0)
        // Выбрав формат передачи данных мы будем ждать
        // определенное кол-во байт
        // и можно будет принимать QByteArray целиком
    {
        QByteArray readString = client->readAll();
        std::cout << "Client says : " << readString.toStdString(); //Выводим в лог

        if (readString == "end\r\n")
            client->close();

           queue->handleResponse(readString);
    }
    if(!queue->isEmpty()){
        QByteArray* byteArray = new QByteArray();
        byteArray = queue->makeRequest(*byteArray);
        client->write(*byteArray);
        delete byteArray;
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
    foreach(QTcpSocket* val, clients.keys())
        {
            val->close();
        }
    close();
}