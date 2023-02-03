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
    this->backMsg = backMsg_;
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