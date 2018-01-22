//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_CONNECT_H
#define CLIENT_COMMON_CONNECT_H
//
// Created by Vetas Ramvet on 17.12.2017.
//
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include <QTimerEvent>
#include "RequestQueue.h"
#include "DiffieHellmanWrapper.h"
#include <QSocketNotifier>

class Connection : public QTcpSocket
{
Q_OBJECT
public:
	
	explicit Connection(QObject *parent = 0);
	~Connection(){
		delete pNot;
	};
	bool connect();
	
	void sendMessage(const QString &message);


signals:
	
	void readyForUse();
	//void newMessage(const QString &from, const QString &message);


private slots:
	
	void read();
	void encryptedRead();
	void writeCredentialsRead();

	void onConnected();
	
	void onDisconnected();
	
	void sendPing();
  	void onData();

protected:
	QByteArray getEncryptedMessage(DiffieHellmanWrapper* wrapper,std::string& msg);
	QByteArray getDecryptedMessage(DiffieHellmanWrapper* wrapper,std::string& msg);
	void timerEvent(QTimerEvent *timerEvent) override;

private:
  	QSocketNotifier* pNot;
	QTimer pingTimer;
	QTime pongTime;
	int transferTimerId;
};

#endif //CLIENT_COMMON_CONNECT_H
