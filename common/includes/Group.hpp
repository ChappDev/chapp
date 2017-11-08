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

#ifndef CHAPP_COMMON_GROUP_H
#define CHAPP_COMMON_GROUP_H

#include "Common.hpp"
#include "Errors.hpp"
#include "User.hpp"
#include <map>
#include <string>
#include <vector>

namespace Chapp {

    using std::string;
    using std::map;
    using std::vector;

    struct Message;

    class Group {
    public:
        Group() = delete;
        virtual ~Group() = default;
        Group(const Group&) = default;
        Group& operator=(const Group&) = default;
        Group(Group&&) = default;
        Group& operator=(Group&&) = default;

    protected:
        /*!
         * Construct group when all params are known
         * @param gid group id
         * @param gname group name
         * @param users map of users in group by their ids
         */
        Group(chapp_id_t gid, GroupType gtype, string gname, map<chapp_id_t, User*> users);

    public:
        /*!
         * Send message coming from uid to all users in group
         * @param uid user id of message author
         * @param msg message to send
         * @return Ok on success
         */
        Error broadcast(chapp_id_t uid, Message msg);

        /*!
         * Invite user to group
         * @param curr_uid inviting user id
         * @param new_uid user id being invited
         * @return Ok on success
         */
        Error invite(chapp_id_t curr_uid, chapp_id_t new_uid);

        /*!
         * Join group with hash
         * @param uid user joining group
         * @param hash hash, which has to be valid for uid
         * @return Ok on success
         */
        Error join(chapp_id_t uid, const Phash& hash);

        /*!
         * Leave group
         * @param uid id of user leaving
         * @return Ok on success
         */
        Error leave(chapp_id_t uid);

        /*!
         * List users in group
         * @return Vector of users in group
         */
        // TODO(stek): Optimize by having cached vector => ret const ref to it?
        // vector<MiniUser> list_users() const;

        /*!
         * Create minigroup representing this group
         * @return
         */
        MiniGroup to_minigroup() const {
            // TODO(stek): Optimize by having cached version
            return {
                    .id = id,
                    .name = name,
                    .type = type,
            };
        };

    private:
        /*!
         * Checks if user is in group already
         * @param uid user id being checked
         * @return true if user is in group already
         */
        inline bool has_user(chapp_id_t uid);

        /*!
         * Generate hash for this uid, checked later via check_hash
         * @param uid user id
         * @return hash guaranteed to be valid for this uid
         */
        virtual Phash gen_hash(chapp_id_t uid) const = 0;

        /*!
         * Check hash for this uid, probably generated via gen_hash
         * @param uid user id
         * @param hash hash recieved from user
         * @return true if hash is valid and user can join, false if not
         */
        virtual bool check_hash(chapp_id_t uid, const Phash& hash) const = 0;

        /*!
         * Create GroupInvite for user
         * @param uid id of user
         * @return Invite valid for user
         */
        GroupInvite make_invite(chapp_id_t uid) const {
            return {
                    .hash = gen_hash(uid),
                    .for_uid = uid,
                    .group = to_minigroup(),
            };
        };

    protected:
        /*!
         * Set last_activity to current timestamp
         */
        void mark_active() {
            last_activity = Util::get_current_ts();
        };

    public:
        chapp_id_t id;
        GroupType type;
        string name;


    protected:
        map<chapp_id_t, User*> users_by_id;
        timestamp_t last_activity;

    };

    class PublicGroup : public Group {
    private:
        friend class GroupFactory; // Only allow to create groups via factory

        /*!
         * Create group
         * @param gid group id, likely generated by db
         * @param gname group name
         * @param creator user creating the group
         */
        PublicGroup(chapp_id_t gid, const string& gname, User* creator);

        /*!
         * Construct group from DB
         * @param gid group id, likely generated by db
         * @param gname group name
         * @param users ids:users in the group
         */
        PublicGroup(chapp_id_t gid, const string& gname, const map<chapp_id_t, User*>& users);

    private:
        /*!
         * Generate hash for uid
         * @return 0-filled hash
         */
        Phash gen_hash(chapp_id_t uid) const override;

        /*!
         * Check hash for uid
         * @return always true
         */
        bool check_hash(chapp_id_t uid, const Phash& hash) const override;

    };

    class ProtectedGroup : public Group {
    private:
        friend class GroupFactory; // Only allow to create groups via factory

        /*!
         * Create group
         * @param gid group id, likely generated by db
         * @param gname group name
         * @param creator user creating group
         * @param hash group password hash
         */
        ProtectedGroup(chapp_id_t gid, const string& gname, User* creator, Phash ghash);

        /*!
         * Construct group
         * @param gid group id, likely generated by db
         * @param gname group name
         * @param users ids:users in the group
         * @param ghash password hash
         */
        ProtectedGroup(chapp_id_t gid, const string& gname, const map<chapp_id_t, User*>& users, Phash ghash);

    private:
        /*!
         * Generate hash
         * @return groups password hash, common for all users
         */
        Phash gen_hash(chapp_id_t uid) const override;

        /*!
         * Check hash
         * @return true if hash matches group's hash
         */
        bool check_hash(chapp_id_t uid, const Phash& hash) const override;

        Phash hash;

    };

    class PrivateGroup : public Group {
    private:
        friend class GroupFactory; // Only allow to create groups via factory

        /*!
         * Create group
         * @param gid group id, likely generated by db
         * @param gname group name
         * @param creator user creating group
         */
        PrivateGroup(chapp_id_t gid, const string& gname, User* creator);

        /*!
         * Construct group
         * @param gid group id, likely generated by db
         * @param gname group name
         * @param users ids:users in the group
         * @param ghash group's hash
         */
        PrivateGroup(chapp_id_t gid, const string& gname, const map<chapp_id_t, User*>& users, Phash ghash);

    private:
        /*!
         * Generate hash
         * @return hash valid for uid
         */
        Phash gen_hash(chapp_id_t uid) const override;

        /*!
         * Check hash
         * @return true if hash is valid for this uid
         */
        bool check_hash(chapp_id_t uid, const Phash& hash) const override;

        Phash hash;

    };

}  // namespace Chapp

#endif
