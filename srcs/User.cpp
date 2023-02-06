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

void User::setMessage(std::string message_) {
    this->message = message_;
}

void    User::setPassword(std::string password_) {
    this->password = password_;
}

void    User::setBackMSG(std::string backMsg_) {
    this->backMsg = backMsg_ + "\n";
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
    this->mode = mode_;
}

void     User::setUnused(std::string unused_) {
    this->unused = unused_;
}

void     User::setRealname(std::string realname_) {
    this->realname = realname_;
}

void    User::setOper(bool oper_) {
    this->oper = oper_;
}