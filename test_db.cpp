//
// Created by George Gabolaev on 02/11/2017.
//

#include "Database.h"

int main(){

    Database obj("localhost", 6379);

    auto userId1 = obj.addUser("gabolaev");
    auto userId2 = obj.addUser("durov");
    auto userId3 = obj.addUser("buryak");
    auto userId4 = obj.addUser("vitalya");
    auto groupId1 = obj.addGroup(Chapp::GroupType::Public, "raggamote", "asldkn");
    auto groupId11 = obj.addGroup(Chapp::GroupType::Public, "alksmda", "wjaknd");
    auto groupId2 = obj.addGroup(Chapp::GroupType::Private, "1l2km3", "alksmd");
    auto groupId22 = obj.addGroup(Chapp::GroupType::Private, "telegram", "1joi23");
    auto groupId3 = obj.addGroup(Chapp::GroupType::Protected, "buros_developement", "op123k");
    auto groupId33 = obj.addGroup(Chapp::GroupType::Protected, "l21lkm3123", "ap9skd");
    std::map<int, std::pair<std::string, Chapp::GroupType>> groups = obj.getListOfGroups();
    for_each(groups.begin(), groups.end(), [](auto _group){
        std::cout << _group.first << " = " << _group.second.first << " = "  << std::endl;
    });
    obj.deleteGroup(PRIVATE, groupId3);
    obj.addUserToGroup(userId1, groupId1);
    obj.addUserToGroup(userId2, groupId2);
    obj.addUserToGroup(userId3, groupId3);
    obj.addUserToGroup(userId4, groupId3);
    obj.deleteUserFromGroup(userId4, groupId3);
    obj.deleteUser(userId4);
    obj.addUserToGroup(userId1, groupId3);
    auto members = obj.getUsersInGroup(groupId3);

    for_each(members.begin(), members.end(), [](auto member){
        std::cout << member << " " << std::endl;

    });
}