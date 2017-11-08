// The MIT License (MIT)
//
// Copyright (c) 2017 Viktor Oreshkin <imselfish@stek29.rocks>
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

/*!
 * @brief Placeholders needed for test build
 *
 * Sample factories
 */

#if !defined(CHAPP_COMMON_PLACEHOLDERS_H) && defined(CHAPP_COMMON_USE_PLACEHOLDERS)
#define CHAPP_COMMON_PLACEHOLDERS_H

#include "Common.hpp"
#include "User.hpp"
#include "Group.hpp"

#include <map>
#include <memory>

namespace Chapp {

    class UserFactory {
    public:
        static UserFactory& getInstance()
        {
            static UserFactory instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }

        UserFactory(const UserFactory&) = delete;
        const UserFactory& operator=(const UserFactory&) = delete;

    private:
        UserFactory() = default;

    public:
        User* by_id(int32_t uid) const {
            auto it = users_by_id.find(uid);

            if (it == users_by_id.end())
                return nullptr;

            return it->second;
        };

        template<class... Args>
        User* construct(Args&&... args) {
            auto user = new User(args...);
            users_by_id.insert(std::make_pair(user->id, user));
            return user;
        }

        ~UserFactory() {
            for (auto &it : users_by_id) {
                delete it.second;
            }
        }

    private:
        map<int32_t, User*> users_by_id;

    };

    class GroupFactory {
    public:
        static GroupFactory& getInstance()
        {
            static GroupFactory instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }

        GroupFactory(const UserFactory&) = delete;
        const GroupFactory& operator=(const UserFactory&) = delete;

    private:
        GroupFactory() = default;

    public:
        Group* by_id(int32_t uid) const {
            auto it = groups_by_id.find(uid);

            if (it == groups_by_id.end())
                return nullptr;

            return it->second;
        };

        template<class... Args>
        Group* construct(GroupType type, Args&&... args) {
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

            groups_by_id.insert(std::make_pair(group->id, group));

            return group;
        }

        ~GroupFactory() {
            for (auto &it : groups_by_id) {
                delete it.second;
            }
        }

    private:
        map<int32_t, Group*> groups_by_id;

    };

}

#endif
