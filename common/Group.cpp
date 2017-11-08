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

#include "Placeholders.hpp"
#include "User.hpp"
#include <utility>

namespace Chapp {

    Group::Group(int32_t gid, string gname, map<int32_t, User*>  users)
            : id(gid)
            , name(std::move(gname))
            , users_by_id(std::move(users))
    {};

    Error Group::broadcast(int32_t  /*uid*/, Message msg) {
        for (const auto &pair : users_by_id) {
            pair.second->deliver_message(msg);
        }

        return Error::Ok;
    }

    Error Group::invite(int32_t curr_uid, int32_t new_uid) {
        if (!has_user(curr_uid)) {
            return Error::NotInGroup; // curr_uid should be in group
        }

        if (has_user(new_uid)) {
            return Error::AlreadyInGroup; // new_uid already in group
        }

        auto new_user = UserFactory::getInstance().by_id(new_uid);

        if (new_user == nullptr) {
            return Error::InvalidUserId; // invalid user
        }

        return new_user->invite(curr_uid, make_invite(new_uid));
    }

    Error Group::join(int32_t uid, const phash& hash) {
        if (!check_hash(uid, hash)) {
            return Error::IncorrectHash; // Wrong hash
        }

        auto user = UserFactory::getInstance().by_id(uid);
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

    Error Group::leave(int32_t uid) {
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

        return Error::Ok;
    }

    bool Group::has_user(int32_t uid) {
        return users_by_id.find(uid) != users_by_id.end();
    }

    PublicGroup::PublicGroup(int32_t gid, const string& gname, User* creator)
            : PublicGroup(gid, gname, { std::make_pair(creator->id, creator) })
    {
        creator->add_to_group(*this);
    }

    PublicGroup::PublicGroup(int32_t gid, const string& gname, const map<int32_t, User*>& users)
            : Group(gid, gname, users)
    {
        type = GroupType::Public;
    }

    phash PublicGroup::gen_hash(int32_t uid) const {
        (void) uid;
        return {};
    }

    bool PublicGroup::check_hash(int32_t uid, const phash & /*hash*/) const {
        (void) uid;
        return true;
    }

    ProtectedGroup::ProtectedGroup(int32_t gid, const string& gname, User* creator, phash ghash)
            : ProtectedGroup(gid, gname, { std::make_pair(creator->id, creator) }, ghash)
    {
        creator->add_to_group(*this);
    }

    ProtectedGroup::ProtectedGroup(int32_t gid, const string& gname, const map<int32_t, User*>& users, phash ghash)
            : Group(gid, gname, users)
            , hash(ghash) {
        type = GroupType::Protected;
    }

    phash ProtectedGroup::gen_hash(int32_t uid) const {
        (void) uid;
        return hash;
    }

    bool ProtectedGroup::check_hash(int32_t uid, const phash &hash) const {
        (void) uid;
        return hash == this->hash;
    }

    PrivateGroup::PrivateGroup(int32_t gid, const string& gname, User* creator)
            : PrivateGroup(gid, gname, { std::make_pair(creator->id, creator) }, gen_rand_phash())
    {
        creator->add_to_group(*this);
    }

    PrivateGroup::PrivateGroup(int32_t gid, const string& gname, const map<int32_t, User*>& users, phash ghash)
            : Group(gid, gname, users)
            , hash(ghash)
    {
        type = GroupType::Private;
    }

    // TODO(stek29): Properly gen/check hashes for uid
    phash PrivateGroup::gen_hash(int32_t uid) const {
        (void) uid;
        return hash;
    }

    bool PrivateGroup::check_hash(int32_t uid, const phash &hash) const {
        (void) uid;
        return hash == this->hash;
    }

} // namespace Chapp
