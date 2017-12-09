#include <QCoreApplication>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "Client.h"
#include "Connection.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    auto nameOfUser = new QString("alexandr");
    auto host = new QString("127.0.0.1");
    QString& hostname = *host;
    quint16 portNumber = 47907;
    auto connection = new Connection(Connection::SocketType::TCP,nameOfUser);
    connection->attachToServer(hostname,portNumber);

    return app.exec();
}