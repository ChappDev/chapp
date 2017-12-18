//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_COMMAND_H
#define CLIENT_COMMON_COMMAND_H


#include <QByteArray>
#include "RequestQueue.h"

class Command {
public:
    virtual QByteArray* req(QByteArray& block, RequestQueue& queue);
    virtual bool res(QByteArray& block, RequestQueue& queue);
};


#endif //CLIENT_COMMON_COMMAND_H
