//
// Created by Vetas Ramvet on 17.12.2017.
//

#include <iostream>

#include <zconf.h>
#include "Connection.h"
#include "DiffieHellmanWrapper.h"

static const int PongTimeout = 60 * 1;

static const int PingInterval = 5 * 1;
void Connection::onData()
{
	qDebug() << Q_FUNC_INFO;
	QTextStream stream(stdin, QIODevice::ReadOnly);
	QString str;
	forever
	{
		fd_set stdinfd;
		FD_ZERO( &stdinfd );
		FD_SET( STDIN_FILENO, &stdinfd );
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		int ready = select( 1, &stdinfd, NULL, NULL, &tv );
		if( ready > 0 )
		{
			str += stream.readLine();
		}
		else
		{
			break;
		}
	}
	qDebug() << "Recieved data:" << str;
	std::string string =str.toStdString();
	write(getEncryptedMessage(DiffieHellmanWrapper::getInstance(),string));
}
Connection::Connection(QObject *parent) : QTcpSocket(parent)
{
	pNot = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
	pNot->setEnabled(false);
	transferTimerId = 0;
	pingTimer.setInterval(PingInterval);
	QObject::connect(this, SIGNAL(readyRead()), this, SLOT(read()));
	QObject::connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
	QObject::connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	QObject::connect(&pingTimer, SIGNAL(timeout()), this, SLOT(sendPing()));
	DiffieHellmanWrapper::getInstance();
	RequestQueue::getInstance()->addCommandToQueue(RequestQueue::Cmd::initDiffieHellman);
}


//Наивная реализация
void Connection::read()
{
	auto queue = RequestQueue::getInstance();
	while (bytesAvailable() > 0)
	{
		QByteArray response = readAll();
		if(!queue->handleResponse(response)){
			QObject::disconnect(this, SIGNAL(readyRead()), this, SLOT(read()));
			QObject::connect(this, SIGNAL(readyRead()), this, SLOT(writeCredentialsRead()));
			writeCredentialsRead();
		}
	}
    if(!queue->isEmpty()){
        QByteArray* byteArray = new QByteArray();
        byteArray = queue->makeRequest(*byteArray);
        write(*byteArray);
    }
}
QByteArray Connection::getEncryptedMessage(DiffieHellmanWrapper* wrapper,std::string& msg){
	return QByteArray::fromStdString(AesEncoder::encrypt(wrapper,msg));
};
QByteArray Connection::getDecryptedMessage(DiffieHellmanWrapper* wrapper,std::string& msg){
	return QByteArray::fromStdString(AesEncoder::decrypt(wrapper,msg));
};
void Connection::writeCredentialsRead(){
	std::string data;
	if(bytesAvailable() > 0) {
		while (bytesAvailable() > 0) {
			QByteArray response = readAll();
			std::string responseData = response.toStdString();
			response = getDecryptedMessage(DiffieHellmanWrapper::getInstance(),responseData);
			responseData = response.toStdString();
			if(response.toStdString() == "p"){
				std::cout << "Введите ваш пароль\n";
				std::cin >> data;
				write(getEncryptedMessage(DiffieHellmanWrapper::getInstance(),data));
				return;
			}
			if(response.toStdString() == "1"){
				//подключаем консоль
				std::cout << "Вы успешно подключились к чату\n";
				QObject::connect(pNot, SIGNAL(activated(int)), this, SLOT(onData()));
				pNot->setEnabled(true);
				QObject::disconnect(this, SIGNAL(readyRead()), this, SLOT(writeCredentialsRead()));
				QObject::connect(this, SIGNAL(readyRead()), this, SLOT(encryptedRead()));
				return;
			}
			else{
				std::cout << "Введенный вам логин уже занят" ;
			}
		}
	}
	std::cout << "Введите ваш логин\n";
	std::cin >> data;
	write(getEncryptedMessage(DiffieHellmanWrapper::getInstance(),data));
}
void Connection::encryptedRead()
{
	while (bytesAvailable() > 0) {
		QByteArray response = readLine();
		std::string responseData = response.toStdString();
		response = getDecryptedMessage(DiffieHellmanWrapper::getInstance(),responseData);
		responseData = response.toStdString();
		std::cout << responseData << "\n";
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