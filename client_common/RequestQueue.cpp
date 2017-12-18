//
// Created by Александр on 09.12.17.
//

#include "RequestQueue.h"
#include "Command.h"
#include "DiffieHellmanInit.h"
void RequestQueue::addCommandToQueue(RequestQueue::Cmd cmd) {
    Command *command = nullptr;
    switch (cmd) {
        case initDiffieHellman:
            command = new DiffieHellmanInit();
            break;
        case calcSharedKey:
            command = new DiffieHellmanInit();
            break;
        default:
            command = new DiffieHellmanInit();
    }
    queueOfRequests->enqueue(command);
}
QByteArray* RequestQueue::makeRequest(RequestQueue::Cmd cmd, QByteArray* block) {
    QByteArray* toResponse = nullptr;
    Command* command = nullptr;
    switch(cmd){
        case initDiffieHellman:
            command = new DiffieHellmanInit();
            toResponse = command->req(block);
            command = new DiffieHellmanInit();
            queueOfRequests->enqueue(command);
            break;
        case calcSharedKey:
            command = new DiffieHellmanInit();
            toResponse = command->req(block);
            queueOfRequests->enqueue(command);
            break;
    }
    return toResponse;
}
void RequestQueue::handleResponse(QByteArray& fromResponse ) {
    if(!queueOfRequests->isEmpty()){
        Command* command = queueOfRequests->dequeue();
        command->res(fromResponse,queueOfRequests);
    }
}
RequestQueue& RequestQueue::getInstance(){
    static RequestQueue* instance;
    if(instance == nullptr){
        instance = new RequestQueue();
    }
    return *instance;
}