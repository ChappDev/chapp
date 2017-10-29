#include <iostream>
#include <QCoreApplication>
#include <QtDebug>
#include "Server.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Server server;
    return app.exec();
}