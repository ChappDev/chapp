[37m//[39;49;00m
[37m// Created by George Gabolaev on 02/11/2017.[39;49;00m
[37m//[39;49;00m

[36m#[39;49;00m[36minclude[39;49;00m [37m"Database.hpp"[39;49;00m[36m[39;49;00m

[36mint[39;49;00m [32mmain[39;49;00m(){

    Database obj([33m"[39;49;00m[33mlocalhost[39;49;00m[33m"[39;49;00m, [34m6379[39;49;00m);

    [34mauto[39;49;00m userId1 = obj.addUser([33m"[39;49;00m[33mgabolaev[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m userId2 = obj.addUser([33m"[39;49;00m[33mdurov[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m userId3 = obj.addUser([33m"[39;49;00m[33mburyak[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m userId4 = obj.addUser([33m"[39;49;00m[33mvitalya[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m groupId1 = obj.addGroup(Chapp::GroupType::Public, [33m"[39;49;00m[33mraggamote[39;49;00m[33m"[39;49;00m, [33m"[39;49;00m[33masldkn[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m result = obj.getGroupInfoById(groupId1);

    [34mauto[39;49;00m groupId11 = obj.addGroup(Chapp::GroupType::Public, [33m"[39;49;00m[33malksmda[39;49;00m[33m"[39;49;00m, [33m"[39;49;00m[33mwjaknd[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m groupId2 = obj.addGroup(Chapp::GroupType::Private, [33m"[39;49;00m[33m1l2km3[39;49;00m[33m"[39;49;00m, [33m"[39;49;00m[33malksmd[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m groupId22 = obj.addGroup(Chapp::GroupType::Private, [33m"[39;49;00m[33mtelegram[39;49;00m[33m"[39;49;00m, [33m"[39;49;00m[33m1joi23[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m groupId3 = obj.addGroup(Chapp::GroupType::Protected, [33m"[39;49;00m[33mburos_developement[39;49;00m[33m"[39;49;00m, [33m"[39;49;00m[33mop123k[39;49;00m[33m"[39;49;00m);
    [34mauto[39;49;00m groupId33 = obj.addGroup(Chapp::GroupType::Protected, [33m"[39;49;00m[33ml21lkm3123[39;49;00m[33m"[39;49;00m, [33m"[39;49;00m[33map9skd[39;49;00m[33m"[39;49;00m);
    std::map<[36mint[39;49;00m, std::pair<std::string, Chapp::GroupType>> groups = obj.getListOfGroups();
    for_each(groups.begin(), groups.end(), []([34mauto[39;49;00m _group){
        std::cout << _group.first << [33m"[39;49;00m[33m = [39;49;00m[33m"[39;49;00m << _group.second.first << std::endl;
    });
    obj.deleteGroup(groupId3);
[37m//    obj.addUserToGroup(userId1, groupId1);[39;49;00m
[37m//    obj.addUserToGroup(userId2, groupId2);[39;49;00m
[37m//    obj.addUserToGroup(userId3, groupId3);[39;49;00m
[37m//    obj.addUserToGroup(userId4, groupId3);[39;49;00m
[37m//    obj.deleteUserFromGroup(userId4, groupId3);[39;49;00m
[37m//    obj.deleteUser(userId4);[39;49;00m
[37m//    obj.addUserToGroup(userId1, groupId3);[39;49;00m
    [34mauto[39;49;00m members = obj.getUsersInGroup(groupId3);

    for_each(members.begin(), members.end(), []([34mauto[39;49;00m member){
        std::cout << member << [33m"[39;49;00m[33m [39;49;00m[33m"[39;49;00m << std::endl;

    });
}
