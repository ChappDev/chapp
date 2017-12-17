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

#ifndef CHAPP_COMMON_USERFACTORY_H
#define CHAPP_COMMON_USERFACTORY_H

#include "Common.hpp"
#include "Group.hpp"
#include "User.hpp"
#include "Database.hpp"

#include <map>
#include <memory>

namespace Chapp {


    class UserFactory {
    public:

        static std::shared_ptr<UserFactory> Instance();

        UserFactory(const UserFactory &) = delete;

        UserFactory &operator=(const UserFactory &) = delete;

        UserFactory(UserFactory &&) = delete;

        UserFactory &operator=(UserFactory &&) = delete;

    private:

        static std::shared_ptr<UserFactory> instance;

        UserFactory() = default;

    public:
        User *by_id(chapp_id_t uid) const {
            auto it = users_by_id.find(uid);

            if (it == users_by_id.end()) {
                return nullptr;
            }

            return it->second;
        };

        template<class... Args>
        User *construct(const std::string &username) {
            int uid = Database::Instance()->addUser(username);
            auto user = new User(uid, username);
            users_by_id.insert(std::make_pair(user->id, user));
            return user;
        }

        ~UserFactory() {
            for (auto &it : users_by_id) {
                delete it.second;
            }
        }

    private:
        map<chapp_id_t, User *> users_by_id;

    };

}  // namespace Chapp

#endif
