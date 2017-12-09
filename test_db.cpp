//
// Created by George Gabolaev on 02/11/2017.
//

#include "Database.hpp"
#include "GroupFactory.hpp"
#include "UserFactory.hpp"
#include "User.hpp"
#include "Group.hpp"

int main(){

    std::shared_ptr<Chapp::GroupFactory> factory(Chapp::GroupFactory::getInstance());

    std::shared_ptr<Database> db(Database::getInstance());

    Chapp::chapp_id_t u1index = db->addUser("gabolaev");
    Chapp::chapp_id_t u2index = db->addUser("tekeev");
    Chapp::chapp_id_t u3index = db->addUser("movsesov");
    Chapp::chapp_id_t u4index = db->addUser("ramazanov");
    Chapp::chapp_id_t u5index = db->addUser("durov");
    Chapp::chapp_id_t u6index = db->addUser("cat_fil");
    Chapp::chapp_id_t u7index = db->addUser("big_cat");

    Chapp::chapp_id_t g1index = db->addGroup(Chapp::GroupType::Private, "raggamote", "sexhashwith20symbols");
    Chapp::chapp_id_t g2index = db->addGroup(Chapp::GroupType::Private, "telegram", "12345678901234567890");

    db->addUserToGroup(u1index, g1index);
    db->addUserToGroup(u2index, g1index);
    db->addUserToGroup(u3index, g1index);
    db->addUserToGroup(u4index, g1index);

    db->addUserToGroup(u5index, g2index);
    db->addUserToGroup(u6index, g2index);
    db->addUserToGroup(u7index, g2index);

    auto group1 = factory->by_id(g1index);
    auto group2 = factory->by_id(g2index);

    std::cout << "test";

}
