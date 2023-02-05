#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.hpp"

class Channel {
public:
    Channel();
    Channel(std::string, User&);
    Channel(std::string, std::string, User&);
    ~Channel();

    void                setName(std::string);
    void                setPass(std::string);
    void                setTopic(std::string);
    void                setMode(std::string);

    void                addUser(int);
    void                addOperator(int);

    std::string         getUsers(std::map<int, User>) const;
    std::string         getOperators(std::map<int, User>) const;

    std::string         getName(void) const { return this->name; };
    std::string         getPass(void) const { return this->pass; };
    std::string         getTopic(void) const { return this->topic; };
    std::string         getMode(void) const { return this->mode; };
private:
    std::string         name;
    std::string         pass;
    std::string         topic;
    std::string         mode;

    std::vector<int>    users;
    std::vector<int>    operators;
};

#endif