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
    return &block;
}
/**
 * reads data from server (prime number)
 * @param array
 * @param queue
 * @return
 */
bool DiffieHellmanInit::res(QByteArray& block, RequestQueue& queue) {
    std::string prime = block.toStdString();
    DiffieHellmanWrapper* wrapper = DiffieHellmanWrapper::getInstance();
    wrapper->checkStringToMpz(prime);
    mpz_class primeMpz;
    primeMpz = prime;
    wrapper->setPrimeNumber(primeMpz);
    queue.addCommandToQueue(RequestQueue::Cmd::calcSharedKey);
    return true;
}