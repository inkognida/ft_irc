#include "../headers/Channel.hpp"


Channel::Channel() {

}

Channel::Channel(std::string name_) {
    this->name = name_;
}

Channel::Channel(std::string name_, std::string pass_) {
    this->name = name_;
    this->pass = pass_;
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

void                Channel::addUser(User &user) {
    this->users.insert(user.getSocket());
    user.setOper(false);
}

void                Channel::addOperator(User &user) {
    this->operators.insert(user.getSocket());
    user.setOper(true);
}

void                Channel::addMode(std::string mode_) {
    this->modes.insert(mode_);
}

std::string         Channel::getUsersInfo(std::map<int, User> &clients) const {
    std::set<int>::const_iterator begin = users.begin();
    std::set<int>::const_iterator end = users.end();

    std::string chUsers;

    while (begin != end) {
        chUsers += clients[*begin].getNickname();
        chUsers += " ";
        begin++;
    }

    return chUsers;
}

std::string         Channel::getOperatorsInfo(std::map<int, User> &clients) const {
    std::set<int>::const_iterator begin = operators.begin();
    std::set<int>::const_iterator end = operators.end();

    std::string chUsers;

    while (begin != end) {
        chUsers += "@";
        chUsers += clients[*begin].getNickname();
        chUsers += " ";
        begin++;
    }

    return chUsers;
}

void         Channel::sendNotificationJoin(std::map<int, User> &clients) {
    std::string usersInfo = getUsersInfo(clients);
    std::string operatorsInfo = getOperatorsInfo(clients);

    std::vector<int>    usersVector(users.begin(), users.end());
    std::vector<int>    operatorsVector(operators.begin(), operators.end());

    for (size_t i = 0; i < usersVector.size(); i++) {
        clients[usersVector[i]].setBackMSG(SERVER + std::to_string(RPL_JOINCHANNEL) + " = " +
            getName() + " " + usersInfo + " " + operatorsInfo);
    }

    for (size_t i = 0; i < operatorsVector.size(); i++) {
        clients[operatorsVector[i]].setBackMSG(SERVER + std::to_string(RPL_JOINCHANNEL) + " = " +
            getName() + " " + operatorsInfo + " " + usersInfo);
    }
}

void        Channel::sendNotificationMsg(std::map<int, User> &clients, std::string msg, User &user) {
    std::vector<int>        usersVector(users.begin(), users.end());
    std::vector<int>        operatorsVector(operators.begin(), operators.end());

    for (size_t i = 0; i < usersVector.size(); i++)
        clients[usersVector[i]].setBackMSG(SERVER + user.getNickname() + "!" + user.getRealname() + "@" +
            user.getHostname() + " " + getName() + ": " + msg);

    for (size_t i = 0; i < operatorsVector.size(); i++)
        clients[operatorsVector[i]].setBackMSG(SERVER + user.getNickname() + "!" + user.getRealname() + "@" +
                                           user.getHostname() + " " + getName() + ": " + msg);
}


void        Channel::sendNotificationTopic(User &user) {
    user.setBackMSG(SERVER + std::to_string(RPL_TOPIC) + " " + user.getCmd() + " = " + getName() + " : " + getTopic());
}

bool        Channel::findMode(std::string mode) const {
    if (modes.find(mode) != modes.end())
        return true;
    else
        return false;
}

bool        Channel::findOper(User &oper) const {
    if (operators.find(oper.getSocket()) != operators.end())
        return true;
    else
        return false;
}

bool        Channel::findUser(User &user) const {
    if (users.find(user.getSocket()) != users.end())
        return true;
    else
        return false;
}

void        Channel::deleteUser(User &user) {
    this->users.erase(user.getSocket());
}

void        Channel::deleteOper(User &user) {
    this->operators.erase(user.getSocket());
}
