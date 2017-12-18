//
// Created by Александр on 09.12.17.
//

#include <QIODevice>
#include <QDataStream>
#include "DiffieHellmanInit.h"
#include "DiffieHellmanWrapper.h"

/**
 * sends group of clients secret pow
 * @param block
 * @return
 */
QByteArray* DiffieHellmanInit::req(QByteArray* block) {
    block->clear();
    block->append("test");
    return block;
}
/**
 * reads data from server (prime number)
 * @param array
 * @param queue
 * @return
 */
bool DiffieHellmanInit::res(QByteArray& block, QQueue<Command*> *queue) {
    std::string prime = block.toStdString();
    DiffieHellmanWrapper wrapper = DiffieHellmanWrapper::getInstance();
    mpz_class primeMpz;
    primeMpz = prime;
    wrapper.setPrimeNumber(primeMpz);
    return false;
}