//
// Created by alexandr movsesov on 30.10.17.
//

#ifndef COMMON_USER_HPP
#define COMMON_USER_HPP

#include <QObject>
#include <QString>
#include <QtCore/QSet>
#include <QtCore/QTime>
#include <groups/GroupCredentials.hpp>
class AbstractGroup;
class InviteGroup;
class Message;
class Session;

class User : public QObject {
public:
    QString* name;
    QSet <InviteGroup*> inviteGroups;
private:
    int id;
    QString* hash;
    QTime lastActionTime = QTime::currentTime();
    Session* session;
    bool sendMessage(AbstractGroup* group, Message* msg);
    bool joinGroup(AbstractGroup* group, GroupCredentials* credentials = nullptr);
};
#endif //COMMON_USER_HPP
