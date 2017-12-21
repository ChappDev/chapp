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

    return &block;
}
void RequestQueue::handleResponse(QByteArray& fromResponse ) {
    if(!queueOfRequests->isEmpty()){
        Command* command = queueOfRequests->dequeue();
        command->res(fromResponse,*this);
    }
}
bool RequestQueue::isEmpty() {
    return queueOfRequests->isEmpty();
}