#ifndef USER_HPP
#define USER_HPP

#include "irc.hpp"

class User {
public:
    User();
    ~User();

    int         getSocket() const { return this->socket; };
    std::string getHostname(void) { return this->hostname; };
    bool        getRegistration(void);

    bool                    getNewUser(void) const { return  this->newUser; }
    void                    setNewUser(bool _new) { this->newUser = _new; }

    std::string                     getPassword() const { return this->password; };
    std::string                     getNickname() const { return this->nickname; };
    std::string                     getUser() const { return this->user; };
    std::string                     getUserMode() const { return this->usermode; };
    std::string                     getUnused() const { return this->unused; };
    std::string                     getRealname() const { return this->realname; };
    std::vector<std::string>        getBackMSG() const { return this->backMsg; };
    std::string                     getResponseMSG();


    std::string                     getCmd() const { return this->cmd; };
    bool                            checkMode(std::string mode_) const {
        if (mode.find(mode_) != mode.end())
            return true;
        return false;
    };


    void                    setSocket(int);
    void                    setHostname(std::string);
    void                    setCmd(std::string);


    void                    setPassword(std::string);
    void                    setNickname(std::string);
    void                    setUser(std::string);
    void                    setUserMode(std::string);
    void                    setUnused(std::string);
    void                    setRealname(std::string);

    void                    setMode(std::string);
    void                    setResponse(std::string);
    void                    setBackMSG(std::string);

    void                    clearBackMSG() { this->backMsg.clear(); };
    void                    clearResponse() { this->response.clear(); };
    void                    deleteMode(std::string mode_) { this->mode.erase(mode_); };


    void                    addChannel(std::string channel) { this->channels.insert(channel); };
    void                    deleteChannel(std::string channel) { this->channels.erase(channel); };

    void                    quitServer(std::map<int, User>&, std::string);
    void                    quitChannels(std::map<std::string, Channel>&);

    bool                    inviteExists(std::string channel) {
        if (invites.find(channel) != invites.end())
            return true;
        return false;
    }
    void                    addInvite(std::string channel) {
        invites.insert(channel);
    }

    void                    deleteInvite(std::string channel) {
        invites.erase(channel);
    }

private:
    int                                         socket;
    std::string                                 hostname;
    std::string                                 cmd;
    bool                                        newUser;

    std::string                                 password;
    std::string                                 nickname;
    std::string                                 user;
    std::string                                 usermode;
    std::string                                 unused;
    std::string                                 realname;

    std::set<std::string>                       mode;
    std::set<std::string>                       channels;
    std::set<std::string>                       invites;

    std::string                                 response;
    std::vector<std::string>                    backMsg;
};

#endif