//
// Created by Александр on 19.12.17.
//

#include <iostream>
#include "DiffieHellmanCalcSharedKey.h"
#include "DiffieHellmanWrapper.h"

QByteArray* DiffieHellmanCalcSharedKey::req(QByteArray& block, RequestQueue& queue) {
    block.clear();
    DiffieHellmanWrapper * wrapper = DiffieHellmanWrapper::getInstance();
    block = block.fromStdString(wrapper->getClientsSecret());
    queue.addCommandToQueue(RequestQueue::Cmd::calcSharedKey);
    return &block;
}
/**
 * reads data from server (secret)
 * @param array
 * @param queue
 * @return
 */
bool DiffieHellmanCalcSharedKey::res(QByteArray& block, RequestQueue& queue) {
    std::string servers_secret = block.toStdString();
    DiffieHellmanWrapper * wrapper = DiffieHellmanWrapper::getInstance();
    wrapper->checkStringToMpz(servers_secret);
    mpz_class secretMpz;
    secretMpz = servers_secret;
    wrapper->setServerExp(secretMpz);
    wrapper->calcSharedSecret();

    return false;
}
