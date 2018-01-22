//
// Created by Александр on 19.12.17.
//

#ifndef SERVER_REQUESTQUEUE_H
#define SERVER_REQUESTQUEUE_H


#include <QByteArray>
#include <QQueue>
#include "DiffieHellmanWrapper.h"
class Command;
class RequestQueue {

 private:
  QQueue<Command*>* queueOfRequests;
 public:

  DiffieHellmanWrapper* wrapper;
  enum Cmd{
    initDiffieHellman,
    calcSharedKey
  };

  RequestQueue(DiffieHellmanWrapper* diffieWrapper){
      queueOfRequests = new QQueue<Command*>();
      wrapper = diffieWrapper;
  };

  ~RequestQueue(){
      delete queueOfRequests;
  }
  /**
   * making request and setting cmd to the queue
   * @param cmd
   * @param queue
   */
  QByteArray* makeRequest(QByteArray& block);
  /**
   * handle response by first command in queue
   * @param queue
   * todo:make templates
   */
  bool handleResponse(QByteArray& fromResponse);
  void addCommandToQueue(RequestQueue::Cmd cmd);
  bool isEmpty();

};

#endif //SERVER_REQUESTQUEUE_H
