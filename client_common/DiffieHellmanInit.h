//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_DIFFIHELLMANCMD_H
#define CLIENT_COMMON_DIFFIHELLMANCMD_H


#include "Command.h"
#include "RequestQueue.h"

class DiffieHellmanInit : public Command {
public:
    explicit DiffieHellmanInit() = default;
    QByteArray* req(QByteArray& block, RequestQueue& queue);
    bool res(QByteArray& array, RequestQueue& queue);
};


#endif //CLIENT_COMMON_DIFFIHELLMANCMD_H
