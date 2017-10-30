//
// Created by alexandr movsesov on 30.10.17.
//

#ifndef INVITEGROUP_HPP
#define INVITEGROUP_HPP


#include "AbstractGroup.hpp"
#include "User.hpp"

class InviteGroup : AbstractGroup {
public:
    bool inviteUser(User* user);
    bool join(User* user);
};


#endif //INVITEGROUP_HPP
