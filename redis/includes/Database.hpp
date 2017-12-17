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

#ifndef CHAPP_DATABASE_H
#define CHAPP_DATABASE_H

#include <cpp_redis/cpp_redis>
#include <Common.hpp>
#include <GroupTypes.hpp>
#include <tuple>

namespace Chapp {
/**
 * Singleton class which provides to
 * call Redis key-value storage main methods
 * needed on groups factory.
 */
    class Database {

    public:

        /**
         * Enum for incrementNowId(typeOfId type) method argument
         * There is 2 now ID in database.
         */
        enum typeOfId {
            GROUP,
            USER
        };

        static std::shared_ptr<Database> Instance();

    private:

        // Disable all the constructors and destructors we don't want to be called.
        Database();

        Database(Database const &) = delete;

        Database &operator=(Database const &) = delete;

    public:

        static std::shared_ptr<Database> instance;

        ~Database() {};

        /**
         * We call this method each time we add a new user or group.
         * @param type What type of ID need to be incremented
         * @return new ID (The String is because it will be immediately used as a query argument to the database.)
         */
        std::string incrementNowId(typeOfId type);

        /**
         *
         * @param type type of group (Public, Protected, Private)
         * @param name name of group
         * @param hash valid hash of this new group ("0" if groups is Public, password hash if group is protected)
         * @return new group ID
         */
        int addGroup(GroupType type, std::string name, std::string hash);

        /**
         *
         * @return map of group_id and pair of group_name group_type
         */
        std::map<int, std::pair<std::string, GroupType>> getListOfGroups();

        /**
         * Removes full information about the group and its users list.
         * @param gid ID of group which has to be deleted
         */
        void deleteGroup(chapp_id_t gid);

        /**
         * Gives full information about group, besides the list of users.
         * @param gid ID of group
         * @return tuple of group_type, group_name, group_hash
         */
        std::tuple<GroupType, std::string, std::string> getGroupInfoById(chapp_id_t gid);

        /**
         * @param username
         * @return new user ID
         */
        int addUser(std::string username);

        /**
         * Removes information about the user, and also TODO deletes it from all groups.
         * @param uid ID of the user whose has to be deleted
         */
        void deleteUser(chapp_id_t uid);

        /**
         * @param uid ID of the user whose nickname we need
         * @return nickname
         */
        std::string getUserNameById(chapp_id_t uid);

        /**
         * Adds the user's ID to the list of members of this group.
         * @param uid ID of user
         * @param gid ID of group
         * @return
         */
        void addUserToGroup(chapp_id_t uid, chapp_id_t gid);

        /**
         * Removes passed user ID from passed group ID members list.
         * @param uid ID of user
         * @param gid ID of group
         */
        void removeUserFromGroup(chapp_id_t uid, chapp_id_t gid);

        /**
         * Gives full list of group members IDs
         * @param gid ID of group, the information about which we need
         * @return
         */
        std::map<chapp_id_t, std::string> getUsersInGroup(chapp_id_t gid);

    private:

        /**
         * Since the list of group members in the database
         * is stored in the form of "group-users:%GROUP_ID%"
         * we need to concatenate that string and group_id we want to request
         * in some methods such as deleteGroup(), addUserToGroup(), deleteUserFromGroup(), getUsersInGroup()
         * @param gid
         * @return
         */
        std::string userInGroupConcat(chapp_id_t gid);

        /**
         * Client from whom we do all requests.
         */
        cpp_redis::client client;

    };

}

#endif //CHAPP_DATABASE_H
