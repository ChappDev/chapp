//
// Created by alexandr movsesov on 30.10.17.
//

#ifndef SESSION_HPP
#define SESSION_HPP

#include <QtCore/QObject>
#include <QtNetwork/QAbstractSocket>
#include "User.hpp"

class Session : QObject {
public:
    void send_serialized(){};
private:
    QString* encryption_key;
    User* user;
    QAbstractSocket* socket;
};


#endif //SESSION_HPP
