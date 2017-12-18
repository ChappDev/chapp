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

class Connection : public QTcpSocket
{
Q_OBJECT
public:
	
	explicit Connection(QObject *parent = 0);
	~Connection();
	bool connect();
	
	void sendMessage(const QString &message);


signals:
	
	void readyForUse();
	//void newMessage(const QString &from, const QString &message);


private slots:
	
	void read();
	
	void onConnected();
	
	void onDisconnected();
	
	void sendPing();

protected:
	
	void timerEvent(QTimerEvent *timerEvent) override;

private:
	QTimer pingTimer;
	QTime pongTime;
	int transferTimerId;
};

#endif //CLIENT_COMMON_CONNECT_H
