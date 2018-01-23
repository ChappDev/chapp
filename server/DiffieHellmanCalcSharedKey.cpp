//
// Created by Александр on 19.12.17.
//

#include <iostream>
#include "DiffieHellmanCalcSharedKey.h"
#include "DiffieHellmanWrapper.h"

QByteArray* DiffieHellmanCalcSharedKey::req(QByteArray& block, RequestQueue& queue) {
    block.clear();
    DiffieHellmanWrapper * wrapper = DiffieHellmanWrapper::getInstance();
    block = block.fromStdString(wrapper->getServersSecret());
    wrapper->calcSharedSecret();
    return &block;
}
/**
 * reads data from server (secret)
 * @param array
 * @param queue
 * @return
 */
bool DiffieHellmanCalcSharedKey::res(QByteArray& block, RequestQueue& queue) {

    return true;
}