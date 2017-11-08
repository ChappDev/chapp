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

Database::Database(std::string ipAddr, int port) : ipAddr(ipAddr), port(port) {

    cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);

    client.connect(ipAddr, port,
                   [](const std::string &host, std::size_t port, cpp_redis::client::connect_state status) {
                       if (status == cpp_redis::client::connect_state::dropped) {
                           std::cout << "client disconnected from " << host << ":" << port << std::endl;
                       }
                   });
}

std::string Database::gidConcat(int gid) {
    return "UIG:"+std::to_string(gid);
}

std::string Database::incrementNowId(typeOfId type) {

    std::string incrementValue = type ? "userNowId" : "groupNowId";
    std::future<cpp_redis::reply> queryRes = client.incr(incrementValue);
    client.sync_commit();
    return std::to_string(queryRes.get().as_integer());
}

int Database::addGroup(Chapp::GroupType type, std::string name, std::string hash) {

    auto newGroupId = incrementNowId(GROUP);
    client.hset(std::to_string(CastFromEnum(type)), newGroupId, name);
    client.hset("hashes", newGroupId, hash);
    client.sync_commit();
    return stoi(newGroupId);
}

std::map<int, std::string> Database::getListOfGroups(Chapp::GroupType type) {

    std::future<cpp_redis::reply> queryRes = client.hgetall(std::to_string(CastFromEnum(type)));
    client.sync_commit();

    auto parsedResponse = queryRes.get().as_array();
    std::map<int, std::string> result = {};

    for (auto iter = parsedResponse.begin(); iter != parsedResponse.end(); iter++) {
        auto key = stoi(iter->as_string());

        if (++iter == parsedResponse.end()) break;

        auto value = iter->as_string();
        result.insert(std::pair<int, std::string>(key, value));
    }
    return result;

}

void Database::deleteGroup(Chapp::GroupType type, int gid) {

    client.hdel(std::to_string(CastFromEnum(type)), {std::to_string(gid)});
    client.sync_commit();

}

int Database::addUser(std::string username) {
    auto newUserId = incrementNowId(USER);
    client.hset("user", newUserId, username);
    client.sync_commit();
    return stoi(newUserId);
}

void Database::deleteUser(int uid){

    client.hdel("user", { std::to_string(uid) });
    client.sync_commit();

}

void Database::addUserToGroup(int uid, int gid) {

    client.sadd("UIG:" + std::to_string(gid), {std::to_string(uid)});
    client.sync_commit();

}

void Database::deleteUserFromGroup(int uid, int gid) {

    client.srem("UIG:" + std::to_string(gid), {std::to_string(uid)});
    client.sync_commit();

}

std::vector<int> Database::getUsersInGroup(int gid) {

    auto queryRes = client.smembers(gidConcat(gid));
    client.sync_commit();
    auto parsedResponse = queryRes.get().as_array();
    std::vector<int> members;
    for_each(parsedResponse.begin(), parsedResponse.end(), [&members](auto _member){
       members.push_back(stoi(_member.as_string()));
    });
    return members;

}


