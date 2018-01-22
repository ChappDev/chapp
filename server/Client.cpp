//
// Created by Александр on 19.12.17.
//

#include "Client.h"
Client::Client() {
    wrapper = new DiffieHellmanWrapper();
    queueOfRequests = new RequestQueue(wrapper);
    user = new User();
}
Client::~Client() {
    delete queueOfRequests;
    delete wrapper;
    delete user;
}