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

#ifndef CHAPP_COMMON_GROUPFACTORY_H
#define CHAPP_COMMON_GROUPFACTORY_H

#include "Common.hpp"
#include "Group.hpp"
#include "User.hpp"

#include <map>
#include <memory>

/*!
 * Example of GroupFactory
 *
 * @code
 * class GroupFactory {
 * public:
 *   GroupFactory& Instance();
 *   Group* get(GroupType type, chapp_id_t gid);
 *   void remove(Group *);
 *
 *   template<class... Args>
 *   Group* construct(GroupType type, Args&&... args);
 * }
 * @endcode
 */

namespace Chapp {

    class GroupFactory {
    public:
        static GroupFactory& Instance()
        {
            static GroupFactory instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }

        GroupFactory(const GroupFactory&) = delete;
        GroupFactory& operator=(const GroupFactory&) = delete;
        GroupFactory(GroupFactory&&) = delete;
        GroupFactory& operator=(GroupFactory&&) = delete;

    private:
        GroupFactory() = default;

    public:
        Group* get(GroupType type, chapp_id_t gid) const {
            auto it = groups_by_id.find(gid);

            if (it == groups_by_id.end()) {
                return nullptr;
            }

            if (it->second->type != type) {
                // Just a safety check
                return nullptr;
            }

            return it->second;
        };

        void remove(Group* group) {
            if (group != nullptr) {
                groups_by_id.erase(group->id);
            }
        }

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
        map<chapp_id_t, Group*> groups_by_id;

    };

}  // namespace Chapp

#endif
