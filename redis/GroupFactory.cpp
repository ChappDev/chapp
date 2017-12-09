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

#include "GroupFactory.hpp"
#include "UserFactory.hpp"
#include "Phash.hpp"
#define unlikely(expr) __builtin_expect(!!(expr),0)
#define IDLE_TIME 1
#define SLEEP_TIME 1

std::mutex locker;

namespace Chapp {

    std::shared_ptr<GroupFactory> GroupFactory::instance;

    std::shared_ptr<GroupFactory> GroupFactory::getInstance() {
        if(!instance) instance.reset(new GroupFactory);
        return instance;
    }

    GroupFactory::GroupFactory() : dbConnect(Database::getInstance()){
        std::cout << "Factory created" << std::endl;
    }

    void GroupFactory::runModerator() {
        std::thread th_mod(&GroupFactory::moderateCachedGroups, this);
    }
    void GroupFactory::moderateCachedGroups() {
        while (true) {
            std::cout << std::endl;
            locker.lock();
            std::set<int> forDelete;
            for (auto iter = groups_by_id.begin(); iter != groups_by_id.end(); iter++) {
                std::cout << time(nullptr) - iter->second->last_activity << std::flush;
                if (time(0) - iter->second->last_activity > IDLE_TIME)
                    forDelete.insert(iter->first);
            }
            for_each(forDelete.begin(), forDelete.end(), [=](int _id){
                remove(_id, false);
            });
            locker.unlock();
            sleep(SLEEP_TIME);
        }
    }

    Group* GroupFactory::by_id(chapp_id_t gid) {
        if (groups_by_id.count(gid)){
            return groups_by_id[gid];
        }
        std::tuple<GroupType, std::string, std::string> result = dbConnect->getGroupInfoById(gid);
        std::map<chapp_id_t , std::string> users = dbConnect->getUsersInGroup(gid);

        std::map<chapp_id_t, User*> usersOfThisGroup;

        for(auto iter = users.begin(); iter != users.end(); iter++){
            usersOfThisGroup.insert({iter->first, UserFactory::Instance().construct(iter->first, iter->second)});
        }

        GroupType type = std::get<0>(result);
        const string groupName(std::get<1>(result));
        Phash newGroupHash(std::get<2>(result));

        auto newGroup = construct(type, gid, groupName, usersOfThisGroup, newGroupHash);
        groups_by_id.insert({gid, newGroup});
        return newGroup;
    }

    void GroupFactory::remove(chapp_id_t gid, bool fromDB) {
        if (fromDB){
            dbConnect->deleteGroup(gid);
        }
        auto it = this->groups_by_id.find(gid);
        delete it->second;
        groups_by_id.erase(it);
    }

    template<class... Args>
    Group *GroupFactory::construct(GroupType type, Args &&... args) {
        Group *group = nullptr;
            switch (type) {
                case GroupType::Public:
                    group = new PublicGroup(args...);
                    break;
                case GroupType::Private:
                    group = new PrivateGroup(args...);
                    break;
                case GroupType::Protected:
                    group = new ProtectedGroup(args...);
                    break;
            }
            return group;
    }

    void GroupFactory::addUserToGroup(chapp_id_t uid, chapp_id_t gid) {
        dbConnect->addUserToGroup(uid, gid);
    }

    void GroupFactory::removeUserFromGroup(chapp_id_t uid, chapp_id_t gid) {
        dbConnect->removeUserFromGroup(uid, gid);
    }


}

