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

#include "Database.hpp"
#define GID_USER "group-users:"
#define UnowID "userNowId"
#define GnowID "groupNowId"
#define GID_NAME "gid-name"
#define GID_HASH "gid-hash"
#define GID_TYPE "gid-type"
#define UID_UNAME "uid-username"
#define USERS "user"
#define DISCONNECT "user disconnected from host"

Database::Database(std::string ipAddr, int port) : ipAddr(ipAddr), port(port) {

    cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);

    client.connect(ipAddr, port,
                   [](const std::string &host, std::size_t port, cpp_redis::client::connect_state status) {
                       if (status == cpp_redis::client::connect_state::dropped) {
                           std::cout << DISCONNECT << host << ":" << port << std::endl;
                       }
                   });
}

std::string Database::userInGroupConcat(uint32_t gid) {
    return GID_USER + std::to_string(gid);
}

std::string Database::incrementNowId(typeOfId type) {

    std::string incrementValue = type ? UnowID : GnowID;
    std::future<cpp_redis::reply> queryRes = client.incr(incrementValue);
    client.sync_commit();
    return std::to_string(queryRes.get().as_integer());
}

int Database::addGroup(Chapp::GroupType type, std::string name, std::string hash) {

    auto newGroupId = incrementNowId(GROUP);
    client.hset(GID_NAME, newGroupId, name);
    client.hset(GID_HASH, newGroupId, hash);
    client.hset(GID_TYPE, newGroupId, std::to_string(CastFromEnum(type)));
    client.sync_commit();
    return stoi(newGroupId);
}

std::tuple<Chapp::GroupType, std::string, std::string> Database::getGroupInfoById(uint32_t gid) {
    std::string idAsString = std::to_string(gid);
    auto nameRequest = client.hget(GID_NAME, idAsString);
    auto hashRequest = client.hget(GID_HASH, idAsString);
    auto typeRequest = client.hget(GID_TYPE, idAsString);
    client.sync_commit();
    return std::tuple<Chapp::GroupType, std::string, std::string>(
            Chapp::CastToEnum<Chapp::GroupType>(stoi(typeRequest.get().as_string())),
            nameRequest.get().as_string(),
            hashRequest.get().as_string()
    );
}

std::map<int, std::pair<std::string, Chapp::GroupType>> Database::getListOfGroups() {

    std::future<cpp_redis::reply> queryResNames = client.hgetall("gid-name");
    client.sync_commit();

    auto parsedResponseNames = queryResNames.get().as_array();

    std::map<int, std::pair<std::string, Chapp::GroupType>> result = {};

    for (auto iter = parsedResponseNames.begin(); iter != parsedResponseNames.end(); iter++) {
        auto key = stoi(iter->as_string());
        auto typeRequest = client.hget(GID_TYPE, iter->as_string());
        client.sync_commit();
        Chapp::GroupType type = Chapp::CastToEnum<Chapp::GroupType>(stoi(typeRequest.get().as_string()));

        if (++iter == parsedResponseNames.end()) break;

        auto value = iter->as_string();
        result.insert(std::pair<int, std::pair<std::string, Chapp::GroupType>>(key, std::pair<std::string, Chapp::GroupType>(value, type)));
    }
    return result;

}

void Database::deleteGroup(uint32_t gid) {

    std::vector<std::string> query = {std::to_string(gid)};
    client.hdel(GID_NAME, query);
    client.hdel(GID_HASH, query);
    client.hdel(GID_TYPE, query);
    client.del({userInGroupConcat(gid)});
    client.sync_commit();

}

int Database::addUser(std::string username) {
    auto newUserId = incrementNowId(USER);
    client.hset(UID_UNAME, newUserId, username);
    client.sync_commit();
    return stoi(newUserId);
}

std::string Database::getUserNameById(uint32_t uid){
    auto nowUserName = client.hget(UID_UNAME, std::to_string(uid));
    client.sync_commit();
    return nowUserName.get().as_string();
}

void Database::deleteUser(uint32_t uid){

    client.hdel(USERS, { std::to_string(uid) });
    client.sync_commit();

}

void Database::addUserToGroup(uint32_t uid, uint32_t gid) {

    client.sadd(userInGroupConcat(gid), {std::to_string(uid)});
    client.sync_commit();

}

void Database::deleteUserFromGroup(uint32_t uid, uint32_t gid) {

    client.srem(userInGroupConcat(gid), {std::to_string(uid)});
    client.sync_commit();

}

std::map<uint32_t, std::string> Database::getUsersInGroup(uint32_t gid) {

    auto queryRes = client.smembers(userInGroupConcat(gid));
    client.sync_commit();
    auto parsedResponse = queryRes.get().as_array();
    std::map<uint32_t, std::string> members;
    for(auto iter = parsedResponse.begin(); iter != parsedResponse.end(); ++iter){
        auto uid = stoi(iter->as_string());
        members.insert(std::pair<uint32_t, std::string>(uid, getUserNameById(uid)));
    }
    return members;

}
