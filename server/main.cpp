#include <QCoreApplication>
#include "Server.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if(!system("clear"))
    {
        system("cls");
    }
    Server server;

    return app.exec();
}