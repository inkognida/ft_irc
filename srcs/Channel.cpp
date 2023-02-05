#include "../headers/Channel.hpp"


Channel::Channel() {

}

Channel::Channel(std::string name_, User &user) {

}

Channel::Channel(std::string name_, std::string pass_, User &user) {

}

Channel::~Channel() {

}

void                Channel::setName(std::string _name) {
    this->name = _name;
}

void                Channel::setPass(std::string _pass) {
    this->pass = _pass;
}

void                Channel::setTopic(std::string _topic) {
    this->topic = _topic;
}

void                Channel::setMode(std::string _mode) {
    this->mode = _mode;
}

void                Channel::addUser(int userSocket) {
    this->users.push_back(userSocket);
}

void                Channel::addOperator(int userSocket) {
    this->operators.push_back(userSocket);
}

std::string         Channel::getUsers(std::map<int, User> clients) const {
    std::vector<int>::iterator begin = users.begin();
    std::vector<int>::iterator end = users.end();

    std::string chUsers;

    while (begin != end) {
        chUsers += clients[*begin].getNickname();
        chUsers += " ";
        begin++;
    }

}

std::string         Channel::getOperators(std::map<int, User> clients) const {
    std::vector<int>::iterator begin = users.begin();
    std::vector<int>::iterator end = users.end();

    std::string chUsers;

    while (begin != end) {
        chUsers += "@";
        chUsers += clients[*begin].getNickname();
        chUsers += " ";
        begin++;
    }
}