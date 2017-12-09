//
// Created by Александр on 09.12.17.
//

#ifndef CLIENT_COMMON_REQUESTCMD_H
#define CLIENT_COMMON_REQUESTCMD_H


#include <QByteArray>
#include <QQueue>
#include "Command.h"

class RequestCmd {

private:
    RequestCmd() = default;

public:
    enum Cmd{
        initDiffiHellman,
    };
    static RequestCmd* getInstance();

    ~RequestCmd(){}
    /**
     * making request and setting cmd to the queue
     * @param cmd
     * @param queue
     */
    QByteArray* makeRequest(RequestCmd::Cmd cmd, QQueue<Command*> *queue,QByteArray* block);
    /**
     * handle response by first command in queue
     * @param queue
     * todo:make templates
     */
    void handleResponse(QQueue<Command*>* queue, QByteArray& fromResponse);

};


#endif //CLIENT_COMMON_REQUESTCMD_H
