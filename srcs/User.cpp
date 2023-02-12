#include "../headers/User.hpp"

User::User() {

}

User::~User() {

}

void User::setSocket(int _socket) {
    this->socket = _socket;
}

void User::setHostname(std::string hostname_) {
    this->hostname = hostname_;
}

void User::setResponse(std::string response_) {
    this->response = response_;
}

void    User::setPassword(std::string password_) {
    this->password = password_;
}

void    User::setBackMSG(std::string backMsg_) {
    this->backMsg.push_back(backMsg_ + "\n");
}

void    User::setCmd(std::string command_) {
    this->cmd = command_;
}

void    User::setNickname(std::string nickname_) {
    this->nickname = nickname_;
}

void    User::setUser(std::string user_) {
    this->user = user_;
}

void     User::setMode(std::string mode_) {
    this->mode.insert(mode_);
}

void    User::setUserMode(std::string usermode_) {
    this->usermode = usermode_;
}

void     User::setUnused(std::string unused_) {
    this->unused = unused_;
}

void     User::setRealname(std::string realname_) {
    this->realname = realname_;
}

void    User::quitChannels(std::map<std::string, Channel>& Channels) {
    std::set<std::string>::const_iterator begin_ = channels.begin();
    std::set<std::string>::const_iterator end_ = channels.end();

    while (begin_ != end_) {
        if (checkMode("+o"))
            Channels[*begin_].deleteOper(*this);
        else
            Channels[*begin_].deleteUser(*this);
        begin_++;
    }
}

void    User::quitServer(std::map<int, User>& Users, std::string reason) {
    std::map<int, User>::iterator begin = Users.begin();
    std::map<int, User>::iterator end = Users.end();

    while (begin != end) {
        begin->second.setBackMSG(getNickname() + "!" + getRealname() + "@" + getHostname() +
                                 std::string(" QUIT ") + reason);
        begin++;
    }
}

std::string User::getResponseMSG() {
    std::vector<std::string>::const_iterator begin = backMsg.begin();
    std::vector<std::string>::const_iterator end = backMsg.end();

    while (begin != end) {
        response += *begin;
        begin++;
    }

    return response;
}

bool        User::getRegistration() {
    if (!getPassword().empty() && !getNickname().empty() && !getUser().empty() &&
        !getUserMode().empty() && !getRealname().empty()) {
        return true;
    }
    return false;
}
