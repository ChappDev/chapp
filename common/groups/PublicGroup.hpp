//
// Created by alexandr movsesov on 30.10.17.
//

#ifndef PUBLICGROUP_HPP
#define PUBLICGROUP_HPP


#include "AbstractGroup.hpp"

class PublicGroup : AbstractGroup {
public:
    bool join(User* user);
};


#endif //PUBLICGROUP_HPP
