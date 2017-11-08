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
#define unlikely(expr) __builtin_expect(!!(expr),0)
#define IDLE_TIME 5
#define SLEEP_TIME 3

std::mutex locker;

namespace Chapp {

    void GroupFactory::moderateCachedGroups() {
        while (true) {
            std::cout << std::endl;
            locker.lock();
            std::set<int> forDelete;
            for (auto iter = groups_by_id.begin(); iter != groups_by_id.end(); iter++) {
                std::cout << time(nullptr) - iter->second.second << std::flush;
                if (unlikely(time(0) - iter->second.second > IDLE_TIME))
                    forDelete.insert(iter->first);
            }
            for_each(forDelete.begin(), forDelete.end(), [=](int _id){
                delete groups_by_id[_id].first;
                groups_by_id.erase(_id);
            });
            locker.unlock();
            sleep(SLEEP_TIME);
        }
    }

    void GroupFactory::remove(Group* group) {
        obj->deleteGroup(group->type, group->id);
    }

    GroupFactory::GroupFactory(){
        obj = new Database("localhost", 6379);
        std::thread th_mod(&GroupFactory::moderateCachedGroups, this);
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
}