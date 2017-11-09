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

#include "Group.hpp"

#include "GroupFactory.hpp"
#include "User.hpp"
#include "UserFactory.hpp"
#include <utility>

namespace Chapp {

    Group::Group(chapp_id_t gid, GroupType gtype, string gname, map<chapp_id_t, User*>  users)
            : id(gid)
            , type(gtype)
            , name(std::move(gname))
            , users_by_id(std::move(users))
            , last_activity(Util::get_current_ts())
    {};

    Error Group::broadcast(chapp_id_t  /*uid*/, Message msg) {
        mark_active();

        for (const auto &pair : users_by_id) {
            pair.second->deliver_message(msg);
        }

        return Error::Ok;
    }

    Error Group::invite(chapp_id_t curr_uid, chapp_id_t new_uid) {
        mark_active();
        
        if (!has_user(curr_uid)) {
            return Error::NotInGroup; // curr_uid should be in group
        }

        if (has_user(new_uid)) {
            return Error::AlreadyInGroup; // new_uid already in group
        }

        auto new_user = UserFactory::Instance().by_id(new_uid);

        if (new_user == nullptr) {
            return Error::InvalidUserId; // invalid user
        }

        return new_user->invite(curr_uid, make_invite(new_uid));
    }

    Error Group::join(chapp_id_t uid, const Phash& hash) {
        mark_active();

        if (!check_hash(uid, hash)) {
            return Error::IncorrectHash; // Wrong hash
        }

        auto user = UserFactory::Instance().by_id(uid);
        if (user == nullptr) {
            return Error::InvalidUserId; // invalid user
        }

        auto ret = user->add_to_group(*this);
        if (ret != Error::Ok) {
            return ret; // failed in add_to_group
        }

        users_by_id.insert(std::make_pair(user->id, user));

        return Error::Ok;
    }

    Error Group::leave(chapp_id_t uid) {
        mark_active();
        
        auto it = users_by_id.find(uid);
        if (it == users_by_id.end()) {
            return Error::NotInGroup; // no such user in group
        }

        auto user = it->second;

        auto ret = user->remove_from_group(*this);
        if (ret != Error::Ok) {
           return ret; // failed in remove_from_group
        }

        users_by_id.erase(it);

        // TODO(stek): This can (and will) blow up one day
        if (users_by_id.empty()) {
            GroupFactory::Instance().remove(this->id, true);
        }

        return Error::Ok;
    }

    bool Group::has_user(chapp_id_t uid) {
        return users_by_id.find(uid) != users_by_id.end();
    }

    PublicGroup::PublicGroup(chapp_id_t gid, const string& gname, User* creator)
            : PublicGroup(gid, gname, { std::make_pair(creator->id, creator) })
    {
        creator->add_to_group(*this);
    }

    PublicGroup::PublicGroup(chapp_id_t gid, const string& gname, const map<chapp_id_t, User*>& users)
            : Group(gid, GroupType::Public, gname, users)
    {}

    Phash PublicGroup::gen_hash(chapp_id_t uid) const {
        (void) uid;
        return {};
    }

    bool PublicGroup::check_hash(chapp_id_t uid, const Phash & /*hash*/) const {
        (void) uid;
        return true;
    }

    ProtectedGroup::ProtectedGroup(chapp_id_t gid, const string& gname, User* creator, Phash ghash)
            : ProtectedGroup(gid, gname, { std::make_pair(creator->id, creator) }, ghash)
    {
        creator->add_to_group(*this);
    }

    ProtectedGroup::ProtectedGroup(chapp_id_t gid, const string& gname, const map<chapp_id_t, User*>& users, Phash ghash)
            : Group(gid, GroupType::Protected, gname, users)
            , hash(ghash)
    {}

    Phash ProtectedGroup::gen_hash(chapp_id_t uid) const {
        (void) uid;
        return hash;
    }

    bool ProtectedGroup::check_hash(chapp_id_t uid, const Phash &hash) const {
        (void) uid;
        return hash == this->hash;
    }

    PrivateGroup::PrivateGroup(chapp_id_t gid, const string& gname, User* creator)
    : PrivateGroup(gid, gname, { std::make_pair(creator->id, creator) }, Phash::RandFilled())
    {
        creator->add_to_group(*this);
    }

    PrivateGroup::PrivateGroup(chapp_id_t gid, const string& gname, const map<chapp_id_t, User*>& users, Phash ghash)
            : Group(gid, GroupType::Private, gname, users)
            , hash(ghash)
    {}

    // TODO(stek): Properly gen/check hashes for uid
    Phash PrivateGroup::gen_hash(chapp_id_t uid) const {
        (void) uid;
        return hash;
    }

    bool PrivateGroup::check_hash(chapp_id_t uid, const Phash &hash) const {
        (void) uid;
        return hash == this->hash;
    }

} // namespace Chapp
