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

#define unlikely(expr) __builtin_expect(!!(expr),0)
#define IDLE_TIME 3
#define SLEEP_TIME 15


namespace Chapp {
    static std::mutex locker;

    std::shared_ptr<GroupFactory> GroupFactory::instance;

    std::shared_ptr<GroupFactory> GroupFactory::Instance() {
        if (!instance) instance.reset(new GroupFactory);
        return instance;
    }

    GroupFactory::GroupFactory() {
        std::cout << "Factory created" << std::endl;
    }

    void GroupFactory::moderateCachedGroups() {
        while (true) {
            std::set<int> forDelete;
            locker.lock();
            for (auto &iter : groups_by_id) {
                if (time(nullptr) - iter.second->last_activity > IDLE_TIME)
                    forDelete.insert(iter.first);
            }
            std::cout << std::endl << "[[[[[[CLEARING]]]]]]" << std::endl;
            for_each(forDelete.begin(), forDelete.end(), [=](int _id) {
                remove(_id, false);
            });
            locker.unlock();
            sleep(SLEEP_TIME);
        }
    }

    void GroupFactory::runModerator() {
        std::thread th_mod(&GroupFactory::moderateCachedGroups, this);
        th_mod.detach();
    }

    // FIXME(stek) Handle non-existent group
    Group *GroupFactory::by_id(chapp_id_t gid) {
        if (groups_by_id.count(gid)) {
            return groups_by_id[gid];
        }

        std::tuple<GroupType, std::string, std::string> result = Database::Instance()->getGroupInfoById(gid);
        std::map<chapp_id_t, std::string> users = Database::Instance()->getUsersInGroup(gid);

        std::map<chapp_id_t, User *> usersOfThisGroup;

        for (auto &user : users) {
            usersOfThisGroup.insert({user.first, UserFactory::Instance()->by_id(user.first)});
        }

        GroupType type = std::get<0>(result);
        const string groupName(std::get<1>(result));
        Phash newGroupHash(std::get<2>(result));

        auto newGroup = construct(type, gid, groupName, usersOfThisGroup, newGroupHash);
        groups_by_id.insert({gid, newGroup});
        return newGroup;
    }


    Group *GroupFactory::create(GroupType type, const string &name, Phash hash) {
        int newGroupId = Database::Instance()->addGroup(type, name, string(hash));
        return by_id(newGroupId);
    }

    void GroupFactory::remove(chapp_id_t gid, bool fromDB) {
        if (fromDB) {
            Database::Instance()->deleteGroup(gid);
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
        Database::Instance()->addUserToGroup(uid, gid);
    }

    void GroupFactory::removeUserFromGroup(chapp_id_t uid, chapp_id_t gid) {
        by_id(gid)->users_by_id.erase(gid);
        Database::Instance()->removeUserFromGroup(uid, gid);
    }

    uint GroupFactory::countOfGroups() {
        return static_cast<uint>(groups_by_id.size());
    }

    void GroupFactory::printFullInfoAboutGroups() {
        for (auto &iter : groups_by_id) {
            std::cout << iter.second->name << " " << iter.second->last_activity << " " << time(nullptr) - iter.second->last_activity << std::endl;
        }
    }

}

