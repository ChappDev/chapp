//
// Created by Александр on 09.12.17.
//

#include "Command.h"
QByteArray* Command::req(QByteArray *block) {
    return block;
}
bool Command::res(QByteArray& block, QQueue<Command *> *queue) {
    return false;
}