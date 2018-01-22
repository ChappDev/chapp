//
// Created by Vetas Ramvet on 29.10.2017.
//

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include "Client.h"
#include "DiffieHellmanWrapper.h"

class AbstractGroup;
class User;
class Message;

class Server : public QTcpServer
{
    Q_OBJECT
public:

    explicit Server(QObject *parent = nullptr);

    /*! @brief stops the server and close
    *
    */
    void sendMsg(QTcpSocket* socket);

    void stop();

    void incomingConnection(qintptr socketDescriptor) override;

    void start(const QHostAddress &address, quint16 port);

    void sendMessageToGroup();

public slots:

    void slotNewConnection();
    void slotServerRead();

  void slotEncryptedRead();
    /*!@brief slotClientDisconnected handles disconnections
    *
    */
    void slotClientDisconnected();
 protected:
  QByteArray getEncryptedMessage(DiffieHellmanWrapper* wrapper,std::string msg);
  QByteArray getDecryptedMessage(DiffieHellmanWrapper* wrapper,std::string msg);
private:

    QMap<QTcpSocket*,Client*> clients;
};


#endif //SERVER_SERVER_HPP
