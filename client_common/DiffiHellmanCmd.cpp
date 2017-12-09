//
// Created by Александр on 09.12.17.
//

#include <QIODevice>
#include <QDataStream>
#include "DiffiHellmanCmd.h"
QByteArray* DiffiHellmanCmd::req(QByteArray* block) {
    QDataStream out(block, QIODevice::WriteOnly);
    out << "test";
    return block;
}
bool DiffiHellmanCmd::res(QByteArray &array, QQueue<Command*> *queue) {
    return false;
}