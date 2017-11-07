//
// Created by George Gabolaev on 02/11/2017.
//

#ifndef CHAPP_DATABASE_H
#define CHAPP_DATABASE_H

#include <cpp_redis/cpp_redis>

enum groupType{
    PUBLIC,
    PRIVATE,
    PROTECTED
};

enum typeOfId{
    GROUP,
    USER
};

class Database {

public:

    Database(std::string ipAddr, int port);

    std::string incrementNowId(typeOfId type);

    int addGroup(groupType type, std::string name);
    std::map<int, std::string> getListOfGroups(bool showPrivateGroups = 0);
    void deleteGroup(groupType type, int gid);

    int addUser(std::string username);
    void deleteUser(int uid);

    void addUserToGroup(int uid, int gid);
    void deleteUserFromGroup(int uid, int gid);
    std::vector<int> getUsersInGroup(int gid);

private:

    std::string gidConcat(int gid);
    std::string ipAddr;
    int port;
    cpp_redis::client client;

};


#endif //CHAPP_DATABASE_H
