//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_DIFFIHELLMANCMD_H
#define CLIENT_COMMON_DIFFIHELLMANCMD_H


#include "Command.h"

class DiffieHellmanCmd : public Command {
public:
    DiffieHellmanCmd() = default;
    QByteArray* req(QByteArray* block);
    bool res(QByteArray& array, QQueue<Command*>* queue);
};


#endif //CLIENT_COMMON_DIFFIHELLMANCMD_H
