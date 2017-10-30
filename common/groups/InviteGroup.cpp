//
// Created by alexandr movsesov on 30.10.17.
//

#include "InviteGroup.hpp"

inline bool InviteGroup::join(User* user) { return false;}
bool InviteGroup::inviteUser(User* user) {
    //TODO::handle iterator from insert method
    users.insert(user);
    return users.contains(user);
}