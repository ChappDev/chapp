//
// Created by Александр on 19.12.17.
//

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H


#include <QTcpSocket>
#include "RequestQueue.h"

struct Client {
    Client();
    ~Client();
    RequestQueue* queueOfRequests;
};


#endif //SERVER_CLIENT_H
