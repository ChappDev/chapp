//
// Created by alexandr movsesov on 30.10.17.
//

#ifndef ABSTRACTGROUP_HPP
#define ABSTRACTGROUP_HPP


#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <Message.hpp>
#include "User.hpp"

class AbstractGroup : QObject {
public:
    AbstractGroup(){};
    ~AbstractGroup(){};
    QString* name;
    virtual bool broadcast (Message* msg) = 0;
    virtual bool join () = 0;
protected:
    int id;
    QSet<User*> users;
};


#endif //ABSTRACTGROUP_HPP
