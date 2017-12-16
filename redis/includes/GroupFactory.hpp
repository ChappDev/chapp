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

#ifndef CHAPP_COMMON_GROUPFACTORY_H
#define CHAPP_COMMON_GROUPFACTORY_H

#include "Common.hpp"
#include "Group.hpp"
#include "User.hpp"
#include "Database.hpp"

#include <map>
#include <memory>
#include <iostream>
#include <thread>
#include <set>
#include <iomanip>
#include <unistd.h>
#include <mutex>

/*!
 * Example of GroupFactory
 *
 * @code
 * class GroupFactory {
 * public:
 *   GroupFactory& Instance();
 *   Group* by_id(chapp_id_t gid);
 *   void remove_by_id(chapp_id_t gid);
 *
 *   template<class... Args>
 *   Group* construct(GroupType type, Args&&... args);
 * }
 * @endcode
 */

namespace Chapp {

    class GroupFactory {
    public:


        static std::shared_ptr<GroupFactory> Instance();

        GroupFactory(const GroupFactory&) = delete;
        GroupFactory& operator=(const GroupFactory&) = delete;
        GroupFactory(GroupFactory&&) = delete;
        GroupFactory& operator=(GroupFactory&&) = delete;

    private:
        std::shared_ptr<Database> dbConnect;
        static std::shared_ptr<GroupFactory> instance;

        GroupFactory();

    public:
        Group* by_id(chapp_id_t gid);

        void runModerator();

        void remove(chapp_id_t gid, bool fromDB);

        void addUserToGroup(chapp_id_t uid, chapp_id_t gid);

        void removeUserFromGroup(chapp_id_t uid, chapp_id_t gid);

        template<class... Args>
        Group* construct(GroupType type, Args&&... args);

        ~GroupFactory() {
            for (auto &it : groups_by_id) {
                delete it.second;
            }
        }

    private:
        void moderateCachedGroups();
        map<chapp_id_t , Group*> groups_by_id;
    };

}  // namespace Chapp

#endif
