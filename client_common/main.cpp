#include <QCoreApplication>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "Connection.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
	
	Connection myСonnection;
	
	std::cout << "Введите порт сервера: ";
	
	quint16 port;
	
	std::cin >> port;
	
	myСonnection.connectToHost("0.0.0.0", port);
	
    return app.exec();
}