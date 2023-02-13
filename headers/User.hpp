#ifndef USER_HPP
#define USER_HPP

#include "irc.hpp"

class User {
public:
    User();
    ~User();

    int                                 getSocket() const { return this->socket; };
    std::string                         getHostname(void) { return this->hostname; };
    bool                                getRegistration(void);

    bool                                getNewUser(void) const { return  this->newUser; }
    void                                setNewUser(bool _new) { this->newUser = _new; }

    std::string                         getPassword() const { return this->password; };
    std::string                         getNickname() const { return this->nickname; };
    std::string                         getUser() const { return this->user; };
    std::string                         getUserMode() const { return this->usermode; };
    std::string                         getUnused() const { return this->unused; };
    std::string                         getRealname() const { return this->realname; };
    std::vector<std::string>            getBackMSG() const { return this->backMsg; };
    std::string                         getResponseMSG();
    std::string                         getCmd() const { return this->cmd; };



    void                                setSocket(int);
    void                                setHostname(std::string);
    void                                setCmd(std::string);


    void                                setPassword(std::string);
    void                                setNickname(std::string);
    void                                setUser(std::string);
    void                                setUserMode(std::string);
    void                                setUnused(std::string);
    void                                setRealname(std::string);

    void                                setResponse(std::string);
    void                                setBackMSG(std::string);

    void                                clearBackMSG() { this->backMsg.clear(); };
    void                                clearResponse() { this->response.clear(); };


    void                                addChannelMode(std::string channel, std::string mode) {
        this->channels[channel].insert(mode);
    };
    bool                                checkChannelMode(std::string channel, std::string mode) {
        if (this->channels[channel].find(mode) != this->channels[channel].end())
            return true;
        return false;
    };
    void                                deleteChannelMode(std::string channel, std::string mode) {
        this->channels[channel].erase(mode);
    };


    void                                addChannel(std::string channel) {
        this->channels[channel];
    };
    void                                deleteChannel(std::string channel) {
        this->channels.erase(channel);
    };
    void                                showLeftChannels() {
        std::map<channel, modes>::const_iterator begin = channels.begin();
        std::map<channel, modes>::const_iterator end = channels.end();

        while (begin != end) {
            setBackMSG(SERVER + std::to_string(RPL_LEFTALLCHANNELS) + " " + begin->first);
            begin++;
        }
    }

    void                                quitServer(std::map<int, User>&, std::string);
    void                                quitChannels(std::map<std::string, Channel>&);

private:
    int                                             socket;
    std::string                                     hostname;
    std::string                                     cmd;
    bool                                            newUser;

    std::string                                     password;
    std::string                                     nickname;
    std::string                                     user;
    std::string                                     usermode;
    std::string                                     unused;
    std::string                                     realname;

    typedef     std::string                         channel;
    typedef     std::set<std::string>               modes;
    std::map<channel, modes>                        channels;


    std::string                                     response;
    std::vector<std::string>                        backMsg;
};

#endif