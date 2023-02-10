#ifndef USER_HPP
#define USER_HPP

#include "irc.hpp"

class User {
public:

    User();
    ~User();

    int         getSocket() const { return this->socket; };
    std::string getHostname(void) { return this->hostname; };

    std::string                     getPassword() const { return this->password; };
    std::string                     getNickname() const { return this->nickname; };
    std::string                     getUser() const { return this->user; };
    std::string                     getUnused() const { return this->unused; };
    std::string                     getRealname() const { return this->realname; };
    std::vector<std::string>        getBackMSG() const { return this->backMsg; };

    std::string                     getResponseMSG() {
        std::vector<std::string>::const_iterator begin = backMsg.begin();
        std::vector<std::string>::const_iterator end = backMsg.end();

        while (begin != end) {
            message += *begin;
            begin++;
        }

        return message;
    };


    std::string                     getCmd() const { return this->cmd; };
    bool                            getOper() const { return this->oper; };
    bool                            checkMode(std::string mode_) const {
        if (mode.find(mode_) != mode.end())
            return true;
        return false;
    };


    void                setSocket(int);
    void                setHostname(std::string);
    void                setCmd(std::string);


    void                setPassword(std::string);
    void                setNickname(std::string);
    void                setUser(std::string);
    void                setMode(std::string);
    void                setUnused(std::string);
    void                setRealname(std::string);

    void                setOper(bool);
    void                setMessage(std::string);
    void                setBackMSG(std::string);

    void                clearBackMSG() { this->backMsg.clear(); };
    void                clearMsg() { this->message.clear(); };
    void                deleteMode(std::string mode_) { this->mode.erase(mode_); };
private:
    int                         socket;
    std::string                 hostname;
    std::string                 cmd;

    std::string                                 password;
    std::string                                 nickname;
    std::string                                 user;
    std::set<std::string>                       mode;
    std::string                                 unused;
    std::string                                 realname;

    bool                                        oper;
    std::string                                 message;
    std::vector<std::string>                    backMsg;
};

#endif