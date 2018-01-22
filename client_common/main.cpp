#include <QCoreApplication>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "Connection.h"
#include <iostream>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc,argv);

	Connection myConnection;

	QTextStream in(stdin);
	std::cout << "Введите адрес сервера: ";
	std::string host;
	getline(std::cin, host);

	std::cout << "Введите порт сервера: ";
	quint16 port;
	std::cin >> port;

	myConnection.connectToHost(QString(host.c_str()), port);

	return app.exec();
}
