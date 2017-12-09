//
// Created by Александр on 09.12.17.
//

#include <QIODevice>
#include <QDataStream>
#include "DiffieHellmanCmd.h"
QByteArray* DiffieHellmanCmd::req(QByteArray* block) {
//    QDataStream out(block, QIODevice::WriteOnly);
    block->clear();
    block->append("test");
//    out << "tttt";
    return block;
}
bool DiffieHellmanCmd::res(QByteArray &array, QQueue<Command*> *queue) {
    return false;
}