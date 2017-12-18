//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_REQUESTCMD_H
#define CLIENT_COMMON_REQUESTCMD_H


#include <QByteArray>
#include <QQueue>
#include "Command.h"

class RequestQueue {

private:
    RequestQueue(){
        queueOfRequests = new QQueue<Command*>();
    };
    QQueue<Command*>* queueOfRequests;
public:
    enum Cmd{
        initDiffieHellman,
        calcSharedKey
    };
    static RequestQueue& getInstance();

    ~RequestQueue(){
        delete queueOfRequests;
    }
    /**
     * making request and setting cmd to the queue
     * @param cmd
     * @param queue
     */
    QByteArray* makeRequest(RequestQueue::Cmd cmd, QByteArray* block);
    /**
     * handle response by first command in queue
     * @param queue
     * todo:make templates
     */
    void handleResponse(QByteArray& fromResponse);
    void addCommandToQueue(RequestQueue::Cmd cmd);

};


#endif //CLIENT_COMMON_REQUESTCMD_H
