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
	auto host  = in.readLine();
	
	std::cout << "Введите порт сервера: ";
	quint16 port;
	std::cin >> port;
	
	myConnection.connectToHost(host, port);
	
    return app.exec();
}