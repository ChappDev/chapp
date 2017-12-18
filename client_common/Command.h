//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_COMMAND_H
#define CLIENT_COMMON_COMMAND_H


#include <QByteArray>

class Command {
public:
    virtual QByteArray* req(QByteArray* block);
    virtual bool res(QByteArray& block, QQueue<Command*>* queue);
};


#endif //CLIENT_COMMON_COMMAND_H
