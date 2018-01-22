//
// Created by Александр on 22.01.18.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <string>
#include <QTime>
struct User {
  std::string name;
  std::string hash;
  QTime timestamp;
  bool is_used;
  User(){
    name = "";
    hash = "";
    timestamp = QTime::currentTime();
    is_used = false;
  }
  void setName(std::string& _name);
  std::string& getHash(std::string& password);
  bool authenticate(std::string& password);
  void setHash(std::string& password);
  bool empty();
  void flush();
};

#endif //SERVER_USER_H
