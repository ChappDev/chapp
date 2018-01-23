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

#include "Database.hpp"
#include "GroupFactory.hpp"
#include "UserFactory.hpp"
#include "User.hpp"
#include "Group.hpp"

static std::mutex locker;

std::string randomWordGenerator(){
    srand(time(NULL));
    std::string result = "";
    for(int i = 0; i < 10+rand()%30; i++){
        result += char(97+rand()%25);
    }
    return result;
}

void creatingGroups() {
    while (true) {
        locker.lock();
        Chapp::GroupFactory::Instance()->create(Chapp::GroupType::Public, randomWordGenerator(), Chapp::Phash("12345679s234567890ss"));
        locker.unlock();
        sleep(1);
    }
}

int main() {

    using namespace Chapp;

//    auto g1 = GroupFactory::Instance()->create(Chapp::GroupType::Public, "raggamote", Phash("12345679s234567890ss"));
//    sleep(3);
//    auto g2 = GroupFactory::Instance()->create(Chapp::GroupType::Public, "one more", Phash("12345679s234567890ss"));
//    sleep(2);
//    auto g3 = GroupFactory::Instance()->create(Chapp::GroupType::Public, "and one", Phash("12345679s234567890ss"));
//    sleep(2);
//    auto g4 = GroupFactory::Instance()->create(Chapp::GroupType::Public, "and two", Phash("12345679s234567890ss"));
//    sleep(2);
//    auto g5 = GroupFactory::Instance()->create(Chapp::GroupType::Public, "ane three", Phash("12345679s234567890ss"));
//    auto gabolaev = UserFactory::Instance()->construct("gabolaev");
//    auto tekeev = UserFactory::Instance()->construct("tekeev");
//    auto movsesov = UserFactory::Instance()->construct("movsesov");
//    auto ramazanov = UserFactory::Instance()->construct("ramazanov");
//
//    raggamote->join(gabolaev->id, Phash("12345679s234567890ss"));
//    raggamote->join(tekeev->id, Phash("12345679s234567890ss"));
//    raggamote->join(movsesov->id, Phash("12345679s234567890ss"));
//    raggamote->join(ramazanov->id, Phash("12345679s234567890ss"));
//
//    auto telegram = GroupFactory::Instance()->create(Chapp::GroupType::Private, "telegram", Phash("uh!@?8&t%>P)1Gjz@ACq"));
//    auto success1 = telegram->join(tekeev->id, Phash("uh!@?8&t%>P)1Gjz@ACq")); //with valid hash
//    auto error = telegram->join(gabolaev->id, Phash("uh!@?8&t%>123Gjz@ACq")); //with wrong hash
//    auto success2 = telegram->join(gabolaev->id, Phash("uh!@?8&t%>P)1Gjz@ACq")); //with valid hash
//
//    raggamote->leave(gabolaev->id);
//    raggamote->leave(tekeev->id);
//    raggamote->leave(movsesov->id);
//    raggamote->leave(ramazanov->id);
//


    GroupFactory::Instance()->runModerator();
    std::thread generator(creatingGroups);
    generator.detach();

    while(true){
        std::cout << std::endl;
        GroupFactory::Instance()->printFullInfoAboutGroups();
        printf("\033[2J\033[1;1H");
        sleep(1);
    }
}
