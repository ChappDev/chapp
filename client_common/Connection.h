//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_CONNECT_H
#define CLIENT_COMMON_CONNECT_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "RequestCmd.h"
#include <QUdpSocket>
#include <cassert>
#include <QDataStream>

class Connection : public QObject {
public:
    enum SocketType{
        TCP,UDP,WEB
    };
    class SocketFactory{
    public:
        static QAbstractSocket* get_by_type(Connection* conn, Connection::SocketType type);
    };
/**
 * slots is a Qt keyword for substitution handlers of signals
 */
private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);
    void onHostFound();

public:
    Connection(SocketType type, QString* newname);
    virtual ~Connection();
    void attachToServer(QString& address, quint16 port);
private:
    QAbstractSocket* socket;
    QString client_name;
    QQueue<Command*>* queueOfRequests;
    quint16 retriviedBytes = 0;
    bool attachSocketSlots();
};


#endif //CLIENT_COMMON_CONNECT_H
