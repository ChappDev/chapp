//
// Created by alexandr movsesov on 30.10.17.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP


#include <QtCore/QString>
#include "User.hpp"
#include "groups/AbstractGroup.hpp"

class Message {
public:
    QString* text;
    User* user;
    AbstractGroup* group;
};


#endif //MESSAGE_HPP
