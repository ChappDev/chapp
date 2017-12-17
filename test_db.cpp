//
// Created by George Gabolaev on 02/11/2017.
//

#include "Database.hpp"
#include "GroupFactory.hpp"
#include "UserFactory.hpp"
#include "User.hpp"
#include "Group.hpp"

int main(){

    using namespace Chapp;

    auto raggamote = GroupFactory::Instance()->create(Chapp::GroupType::Public, "raggamote", Phash("12345679s234567890ss"));
    auto gabolaev = UserFactory::Instance()->construct("gabolaev");
    auto tekeev = UserFactory::Instance()->construct("tekeev");
    auto movsesov = UserFactory::Instance()->construct("movsesov");
    auto ramazanov = UserFactory::Instance()->construct("ramazanov");

    raggamote->join(gabolaev->id, Phash("12345679s234567890ss"));
    raggamote->join(tekeev->id, Phash("12345679s234567890ss"));
    raggamote->join(movsesov->id, Phash("12345679s234567890ss"));
    raggamote->join(ramazanov->id, Phash("12345679s234567890ss"));

    auto telegram = GroupFactory::Instance()->create(Chapp::GroupType::Private, "telegram", Phash("uh!@?8&t%>P)1Gjz@ACq"));
    auto success1 = telegram->join(tekeev->id, Phash("uh!@?8&t%>P)1Gjz@ACq")); //with valid hash
    auto error = telegram->join(gabolaev->id, Phash("uh!@?8&t%>123Gjz@ACq")); //with wrong hash
    auto success2 = telegram->join(gabolaev->id, Phash("uh!@?8&t%>P)1Gjz@ACq")); //with valid hash

    raggamote->leave(gabolaev->id);
    raggamote->leave(tekeev->id);
    raggamote->leave(movsesov->id);
    raggamote->leave(ramazanov->id);

    std::cout << "done";
}
