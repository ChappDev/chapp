//
// Created by Александр on 09.12.17.
//

#include "RequestQueue.h"
#include "Command.h"
#include "DiffieHellmanInit.h"
#include "DiffieHellmanCalcSharedKey.h"
class Command;
void RequestQueue::addCommandToQueue(RequestQueue::Cmd cmd) {
    Command *command = nullptr;
    switch (cmd) {
        case initDiffieHellman:
            command = new DiffieHellmanInit();
            break;
        case calcSharedKey:
            command = new DiffieHellmanCalcSharedKey();
            break;
        default:
            command = new DiffieHellmanInit();
    }
    queueOfRequests->enqueue(command);
}
QByteArray* RequestQueue::makeRequest(QByteArray& block) {
    Command* command = nullptr;
    if(!queueOfRequests->isEmpty()){
        command = queueOfRequests->dequeue();
        block = *command->req(block,*this);
    }
    if(command) {
        delete command;
    }
    return &block;
}
bool RequestQueue::handleResponse(QByteArray& fromResponse ) {
    Command* command = nullptr;
    bool resp = false;
    if(!queueOfRequests->isEmpty()){
        command = queueOfRequests->dequeue();
        resp = command->res(fromResponse,*this);
    }
    if(command) {
        delete command;
    }
    return resp;
}
RequestQueue* RequestQueue::getInstance(){
    static RequestQueue* instance;
    if(instance == nullptr){
        instance = new RequestQueue();
    }
    return instance;
}
bool RequestQueue::isEmpty() {
    return queueOfRequests->isEmpty();
}