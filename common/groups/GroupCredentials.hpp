//
// Created by alexandr movsesov on 30.10.17.
//

#ifndef GROUPCREDENTIALS_HPP
#define GROUPCREDENTIALS_HPP


#include <QtCore/QObject>
/*
 * Some interface for encapsulating hashing logic
 * Used for creating Private Groups
 */
class GroupCredentials : QObject {
public:
    //TODO::implement abstract hashing algorithm and set it in second param
    explicit GroupCredentials(QString* password){
        hash = password;
    };
    ~GroupCredentials(){
        delete hash;
    }
private:
    QString* hash;
};


#endif //GROUPCREDENTIALS_HPP
