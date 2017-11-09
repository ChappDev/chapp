// The MIT License (MIT)
//
// Copyright (c) 2017 George Gabolaev <gabolaev98@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef CHAPP_DATABASE_H
#define CHAPP_DATABASE_H

#include <cpp_redis/cpp_redis>
#include <Common.hpp>
#include <GroupTypes.hpp>
#include <tuple>

enum typeOfId{
    GROUP,
    USER
};

class Database {

public:

    //TODO: 1) add auth by password; 2) protected groups passwords; 3) protected groups invite-lists;

    Database(std::string ipAddr, int port);

    std::string incrementNowId(typeOfId type);

    int addGroup(Chapp::GroupType type, std::string name, std::string hash);
    std::map<int, std::pair<std::string, Chapp::GroupType>> getListOfGroups();
    void deleteGroup(uint32_t gid);
    std::tuple<Chapp::GroupType, std::string, std::string> getGroupInfoById(uint32_t uid);

    int addUser(std::string username);
    void deleteUser(uint32_t uid);
    std::string getUserNameById(uint32_t uid);

    void addUserToGroup(uint32_t uid, uint32_t gid);
    void deleteUserFromGroup(uint32_t uid, uint32_t gid);
    std::map<uint32_t, std::string> getUsersInGroup(uint32_t gid);
    //TODO GET GROUP HASH
private:

    std::string userInGroupConcat(uint32_t gid);
    std::string ipAddr;
    int port;
    cpp_redis::client client;

};


#endif //CHAPP_DATABASE_H
