#include <QCoreApplication>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "Client.h"
#include "Connection.h"

int main(int argc, char *argv[])
{
    auto nameOfUser = new QString("alexandr");
    auto host = new QString("0.0.0.0");
    QString& hostname = *host;
    quint16 portNumber = 1306;
    auto connection = new Connection(Connection::SocketType::TCP,nameOfUser);
    connection->attachToServer(hostname,portNumber);
    QCoreApplication app(argc,argv);

    return app.exec();
}