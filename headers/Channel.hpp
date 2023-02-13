#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.hpp"

class Channel {
public:
    Channel();
    Channel(std::string);
    Channel(std::string, std::string);
    ~Channel();

    void                setName(std::string);
    void                setPass(std::string);
    void                setTopic(std::string);

    void                addMode(std::string);
    void                deleteMode(std::string);

    void                addUser(User&);
    void                addOperator(User&);
    void                deleteUser(User&);
    void                deleteOper(User&);

    std::string                 getUsersInfo(std::map<int, User>&) const;
    std::string                 getOperatorsInfo(std::map<int, User>&) const;
    void                        sendNotificationJoin(std::map<int, User>&);
    void                        sendNotificationTopic(User&);
    void                        sendNotificationMsg(std::map<int, User> &clients, std::string, User&);

    std::string                 getName(void) const { return this->name; };
    std::string                 getPass(void) const { return this->pass; };
    std::string                 getTopic(void) const { return this->topic; };
    std::set<std::string>       getModes(void) const { return this->modes; };

    bool                        findMode(std::string) const;
    bool                        findOper(User&) const;
    bool                        findUser(User&) const;
    void                        clearTopic(void) { this->topic.clear(); };
private:
    std::string                         name;
    std::string                         pass;
    std::string                         topic;
    std::set<std::string>               modes;

    std::set<int>                       users;
    std::set<int>                       operators;
};

#endif