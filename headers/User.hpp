#ifndef USER_HPP
#define USER_HPP

#include "irc.hpp"

class User {
public:

    User();
    ~User();

    int         getSocket() const { return this->socket; };
    std::string getHostname(void) { return this->hostname; };

    std::string getPassword() const { return this->password; };
    std::string getNickname() const { return this->nickname; };
    std::string getUser() const { return this->user; };
    std::string getMessage() const { return this->message; };
    std::string getBackMSG() const { return this->backMsg; };
    std::string getCmd() const { return this->cmd; };



    void                setSocket(int);
    void                setHostname(std::string);
    void                setCmd(std::string);


    void                setPassword(std::string);
    void                setNickname(std::string);
    void                setUser(std::string);
    void                setMessage(std::string);
    void                setBackMSG(std::string);
private:
    int                         socket;
    std::string                 hostname;
    std::string                 cmd;

    std::string                 password;
    std::string                 nickname;
    std::string                 user; // TODO user mode unused realname, mode, etc...

    std::string                 message;
    std::string                 backMsg;

};

#endif