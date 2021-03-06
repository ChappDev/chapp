// The MIT License (MIT)
//
// Copyright (c) 2017 Viktor Oreshkin <imselfish@stek29.rocks>
//                    George Gabolaev <gabolaev98@gmail.com>
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

#include "User.hpp"

#include "Group.hpp"
#include "GroupFactory.hpp"
#include "UserFactory.hpp"
#include <chapp.pb.h>
#include <utility>

namespace Chapp {

    User::User(chapp_id_t uid, const string& username)
        : User(uid, username, Phash::RandFilled()) {};

    User::User(chapp_id_t uid, string username, Phash hash)
            : id(uid)
            , username(std::move(username))
            , pass_hash(hash)
            , last_activity(Util::get_current_ts())
    {};

    User::~User() {
        auto groupFactory = GroupFactory::Instance();
        for (auto &group_id : joined_groups) {
            // TODO(stek): Avoid constructing groups just to delete user?

            groupFactory->removeUserFromGroup(this->id, group_id);

            // TODO(stek): When socket is added, avoid notifying it here
        }
    }

    void User::deliver_message(const Message& msg) {
        // TODO(stek): send to socket
        (void) msg;
    }

    MiniUser User::to_miniuser() const {
        // TODO(stek): Optimize by having cached version
        API::User user;
        user.set_id((google::protobuf::uint32) id);
        user.set_username(username);
        return user;
    };

    Error User::invite(chapp_id_t inviter_id, const GroupInvite& invite) {
        auto gid = invite.group.id;

        auto it = invites_by_gid.find(gid);

        if (it != invites_by_gid.end()) {
            return Error::AlreadyInvited; // already invited
        }

        invites_by_gid.insert(std::make_pair(gid, invite));

        // TODO(stek): notify socket
        (void) inviter_id;

        return Error::Ok;
    };

    Error User::add_to_group(const Group &group) {
        auto insert_pair = joined_groups.insert(group.id);
        if (!insert_pair.second) {
            return Error::AlreadyInGroup; // Not inserted
        }

        GroupFactory::Instance()->addUserToGroup(this->id, group.id);

        // TODO(stek): notify socket
        return Error::Ok;
    }

    Error User::remove_from_group(const Group &group) {
        auto removed_cnt = joined_groups.erase(group.id);
        if (removed_cnt == 0) {
            return Error::NotInGroup; // Not erased
        }

        GroupFactory::Instance()->removeUserFromGroup(this->id, group.id);

        return Error::Ok;
    }

}  // namespace Chapp
