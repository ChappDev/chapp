//
// Created by alexandr movsesov on 30.10.17.
//

#ifndef PRIVATEGROUP_HPP
#define PRIVATEGROUP_HPP

#include "AbstractGroup.hpp"
#include "GroupCredentials.hpp"

/*
 * Password Group
 */
class PrivateGroup : AbstractGroup {
public:
    bool join(GroupCredentials* credentials);
protected:
    GroupCredentials* credentials;
};


#endif //PRIVATEGROUP_HPP
