//
// Created by Vetas Ramvet on 17.12.2017.
//

#include "Connection.h"
#include "DiffieHellmanWrapper.h"

static const int PongTimeout = 60 * 1;

static const int PingInterval = 5 * 1;

Connection::Connection(QObject *parent) : QTcpSocket(parent)
{
	
	transferTimerId = 0;
	pingTimer.setInterval(PingInterval);

	QObject::connect(this, SIGNAL(readyRead()), this, SLOT(read()));
	QObject::connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
	QObject::connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	QObject::connect(&pingTimer, SIGNAL(timeout()), this, SLOT(sendPing()));
	DiffieHellmanWrapper::getInstance();
	RequestQueue::getInstance().addCommandToQueue(RequestQueue::Cmd::initDiffieHellman);
}
Connection::~Connection(){
}
//Наивная реализация
void Connection::read()
{
	auto queue = RequestQueue::getInstance();
	while (bytesAvailable() > 0)
	{
		QByteArray response = readAll();
		queue.handleResponse(response);
		//qDebug() << readAll();
	}
}

void Connection::onConnected()
{
	qDebug() << "Connected";
}

void Connection::onDisconnected()
{
	qDebug() << "Disconnected";
}

void Connection::sendPing()
{
	if (pongTime.elapsed() > PongTimeout)
	{
		abort();
		return;
	}
	
	write("ping");
}

void Connection::timerEvent(QTimerEvent *timerEvent)
{
	if (timerEvent->timerId() == transferTimerId)
	{
		abort();
		killTimer(transferTimerId);
		transferTimerId = 0;
	}
}

void Connection::sendMessage(const QString &message)
{
	if (message.isEmpty())
		return;
	
	QByteArray msg = message.toUtf8();
	write(msg);
}