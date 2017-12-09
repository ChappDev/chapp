//
// Created by Александр on 09.12.17.
//

#include "RequestCmd.h"
#include "Command.h"
#include "DiffieHellmanCmd.h"

QByteArray* RequestCmd::makeRequest(RequestCmd::Cmd cmd, QQueue<Command*> *queue,QByteArray* block) {
    QByteArray* toResponse = nullptr;
    switch(cmd){
        case initDiffieHellman:
            Command* command = new DiffieHellmanCmd();
            toResponse = command->req(block);
            queue->enqueue(command);
            break;
    }
    return toResponse;
}
void RequestCmd::handleResponse(QQueue<Command*> *queue, QByteArray& fromResponse ) {
    if(!queue->isEmpty()){
        Command* command = queue->dequeue();
        command->res(fromResponse,queue);
        delete(command);
    }
}
RequestCmd* RequestCmd::getInstance(){
    static RequestCmd* instance;
    if(instance == nullptr){
        instance = new RequestCmd();
    }
    return instance;
}