//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_DIFFIHELLMANCMD_H
#define CLIENT_COMMON_DIFFIHELLMANCMD_H


#include "Command.h"

class DiffiHellmanCmd : public Command {
public:
    DiffiHellmanCmd() = default;
    QByteArray* req(QByteArray* block);
    bool res(QByteArray& array, QQueue<Command*>* queue);
};


#endif //CLIENT_COMMON_DIFFIHELLMANCMD_H
