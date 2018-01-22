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
#include "AesEncoder.h"

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

<<<<<<< HEAD
  void broadcast(QByteArray &message);
=======
    void broadcast(QByteArray &message);

public slots:
>>>>>>> 070243558ddaf43f93fc419d1c7ad5f2c1034d5f

 public slots:

  void slotNewConnection();
  void slotServerRead();
  void slotClientCredentialsRead();
  void slotEncryptedRead();
  /*!@brief slotClientDisconnected handles disconnections
  *
  */
  void slotClientDisconnected();
 protected:
  QByteArray getEncryptedMessage(DiffieHellmanWrapper* wrapper,std::string msg);
  QByteArray getDecryptedMessage(DiffieHellmanWrapper* wrapper,std::string msg);
  bool checkUsers(std::string& name, std::string &pass);
 private:

  QMap<QTcpSocket*,Client*> clients;
  QMap<std::string,User*> users;
};


#endif //SERVER_SERVER_HPP