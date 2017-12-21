//
// Created by Александр on 09.12.17.
//

#include "Command.h"
QByteArray* Command::req(QByteArray& block, RequestQueue& queue) {
    return &block;
}
bool Command::res(QByteArray& block, RequestQueue& queue) {
    return false;
}