//
// Created by Александр on 19.12.17.
//

#ifndef CLIENT_COMMON_DIFFIEHELLMANCALCSHAREDKEY_H
#define CLIENT_COMMON_DIFFIEHELLMANCALCSHAREDKEY_H



#include "Command.h"
#include "RequestQueue.h"

class DiffieHellmanCalcSharedKey : public Command {
public:
    explicit DiffieHellmanCalcSharedKey() = default;
    QByteArray* req(QByteArray& block, RequestQueue& queue);
    bool res(QByteArray& array, RequestQueue& queue);
};



#endif //CLIENT_COMMON_DIFFIEHELLMANCALCSHAREDKEY_H
