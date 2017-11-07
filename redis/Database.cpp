//
// Created by George Gabolaev on 02/11/2017.
//


#include "Database.h"


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

int Database::addGroup(groupType type, std::string name) {

    auto newGroupId = incrementNowId(GROUP);
    client.hset(std::to_string(type), newGroupId, name);
    client.sync_commit();
    return stoi(newGroupId);
}

std::map<int, std::string> Database::getListOfGroups(bool showPrivateGroups) {

    std::future<cpp_redis::reply> queryRes = client.hgetall(std::to_string(showPrivateGroups));
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

void Database::deleteGroup(groupType type, int gid) {

    client.hdel(std::to_string(type), {std::to_string(gid)});
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

