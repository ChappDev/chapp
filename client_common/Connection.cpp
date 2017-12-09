//
// Created by Александр on 09.12.17.
//

#include "Connection.h"
/**
 * Simple socket builder
 * @param conn
 * @param type
 * @return
 */
QAbstractSocket* Connection::SocketFactory::get_by_type(Connection* conn, Connection::SocketType type) {
    switch(type){
        case TCP:
            return new QTcpSocket(conn);
        case UDP:
            return new QUdpSocket(conn);
        default:
            return nullptr;
        //todo(tuman): implement web sockets: return new QWebSocket
    }
}
//todo(tuman): implement client`s ability to use proxy
/**
 * Constructor, attaches slots
 * @param type
 * @param newname
 */
Connection::Connection(SocketType type, QString *newname) {
    socket = SocketFactory::get_by_type(this,type);
    assert(attachSocketSlots());
    client_name = *newname;
    queueOfRequests = new QQueue<Command*>();
}
/**
 * Desctructor
 */
Connection::~Connection() {
    delete(socket);
}
/**
 * attach standart socket signals to our handlers
 * @return
 */
bool Connection::attachSocketSlots()
{
    bool is_connected = true;
    is_connected = bool(connect(socket, &QTcpSocket::readyRead, this, &Connection::onReadyRead));
    is_connected = is_connected?bool(connect(socket, &QTcpSocket::connected, this, &Connection::onConnected)):false;
    is_connected = is_connected?bool(connect(socket, &QTcpSocket::disconnected, this, &Connection::onDisconnected)):false;
    is_connected = is_connected?bool(connect(socket, &QTcpSocket::hostFound,this, &Connection::onHostFound)):false;
    //is_connected = is_connected?bool(connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(Connection::onError(QAbstractSocket::SocketError)))):false;
    return is_connected;
}
void Connection::attachToServer(QString& address, quint16 port) {
    socket->connectToHost(address, port);
}
/**
 * Connection was established
 * start d.h. for making secure link
 * need to think about how server parse string
 */
void Connection::onConnected() {
    RequestCmd* cmd = RequestCmd::getInstance();
    auto byteArray = new QByteArray();
    byteArray = cmd->makeRequest(RequestCmd::initDiffiHellman,queueOfRequests,byteArray);
    socket->write(*byteArray);
    delete(byteArray);
}
/**
 * Handle server`s response, we need to have some sort of order for handling server response
 */
void Connection::onReadyRead() {
    QDataStream in(socket);
    if (retriviedBytes == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> retriviedBytes;
    }
    qDebug() << retriviedBytes;
}

/**
 * simple mark about network layer
 */
void Connection::onHostFound() {
    qDebug() << "server host: "
             << socket->peerAddress()
             << " is able to connect\n";

}

/**
 * disconnected handler
 */
void Connection::onDisconnected() {
    qDebug() << "connection lost";
}

/**
 * simple error logger
 * todo::implement logging subsystem
 * @param socketError
 */
void Connection::onError(QAbstractSocket::SocketError socketError) {
    qDebug() << "some issues with the network, SocketError number: "
             << socketError
             << "\n";
}