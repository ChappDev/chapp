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

#ifndef CHAPP_COMMON_USER_H
#define CHAPP_COMMON_USER_H

#include <ctime>
#include <map>
#include <set>
#include <string>

#include "Common.hpp"
#include "Errors.hpp"
#include "Session.hpp"


namespace Chapp {
    using std::string;
    using std::map;
    using std::set;

    class Group;

    class User {
    public:
        User() = delete;
        User(const User&) = delete;
        User& operator=(const User&) = delete;
        User(User &&) = delete;
        User& operator=(User &&) = delete;

        ~User();

    private:
        friend class UserFactory; // only allow construct users via UserFactory
        friend class GroupFactory;

        User(chapp_id_t uid, const string& username);
        User(chapp_id_t uid, string username, Phash hash);

    public:
        /*!
         * Deliver message to user
         * @param msg message to send
         */
        void deliver_message(Message msg);

        /*!
         * @brief Invite user to group
         * @note use group's invite method, don't use this directly
         * @param inviter_id id of user initiating invite
         * @param invite GroupInvite for group user is being invited to
         * @return Ok on success
         */
        Error invite(chapp_id_t inviter_id, const GroupInvite& invite);

        /*!
         * Add user to group
         * @param group Group to add to user's groups
         * @return Ok on success
         */
        Error add_to_group(const Group& group);

        /*!
         * Remove user from group
         * @param group Group to remove from user's groups
         * @return Ok on success
         */
        Error remove_from_group(const Group& group);
        // TODO(stek): add "unsafe/fast" version to be called from destructor and safe version

        /*!
         * Create minigroup representing this group
         * @return
         */
        MiniUser to_miniuser() const {
            // TODO(stek): Optimize by having cached version
            return {
                    .id = id,
                    .username = username,
            };
        };

    private:
        /*!
         * Set last_activity to current timestamp
         */
        void mark_active() {
            last_activity = Util::get_current_ts();
        };

    public:
        chapp_id_t id;
        string username;

    private:
        Phash pass_hash;
        map<chapp_id_t, GroupInvite> invites_by_gid;
        set<chapp_id_t> joined_groups;
        timestamp_t last_activity;

    };

}  // namespace Chapp

#endif
