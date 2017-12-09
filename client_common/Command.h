//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_COMMAND_H
#define CLIENT_COMMON_COMMAND_H


#include <QByteArray>

class Command {
public:
    QByteArray* req(QByteArray* block);
    bool res(QByteArray& array, QQueue<Command*>* queue);
};


#endif //CLIENT_COMMON_COMMAND_H
