//
// Created by Александр on 19.12.17.
//

#include "Client.h"
Client::Client() {
    queueOfRequests = new RequestQueue();
}
Client::~Client() {
    delete queueOfRequests;
}