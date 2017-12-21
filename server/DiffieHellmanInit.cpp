//
// Created by Александр on 09.12.17.
//

#include <QIODevice>
#include <QDataStream>
#include "DiffieHellmanInit.h"
#include "DiffieHellmanWrapper.h"
#include "RequestQueue.h"

/**
 * sends group of clients secret pow
 * @param block
 * @return
 */
QByteArray* DiffieHellmanInit::req(QByteArray& block, RequestQueue& queue) {
    block.clear();
    DiffieHellmanWrapper* wrapper = DiffieHellmanWrapper::getInstance();
    block = block.fromStdString(wrapper->getPrimeNumber());
    queue.addCommandToQueue(RequestQueue::Cmd::initDiffieHellman);
    return &block;
}
/**
 * reads data from server (prime number)
 * @param array
 * @param queue
 * @return
 */
bool DiffieHellmanInit::res(QByteArray& block, RequestQueue& queue) {
    std::string clientsExp = block.toStdString();
    DiffieHellmanWrapper* wrapper = DiffieHellmanWrapper::getInstance();
    clientsExp = wrapper->checkStringToMpz(clientsExp);
    mpz_class clientsExpMpz;
    clientsExpMpz = clientsExp;
    wrapper->setClientExp(clientsExpMpz);
    queue.addCommandToQueue(RequestQueue::Cmd::calcSharedKey);
    return true;
}