#include "../headers/User.hpp"

User::User() {
    commandsAvaliable.push_back("PASS");
}

User::~User() {

}

std::string User::getNickname() const {
    return this->nickname;
}

int User::getSocket() const {
    return this->socket;
}

void User::setSocket(int _socket) {
    this->socket = _socket;
}

void User::setHostname(std::string hostname_) {
    this->hostname = hostname_;
}

std::string User::getHostname(void) {
    return this->hostname;
}

void User::setMessage(std::string message_) {
    this->message = message_;
}

std::string User::getMessage() {
    return this->message;
}


void User::setCommandsToParse(std::string input) {
    std::string tmp;
    std::stringstream ss(input);

    while (getline(ss, tmp, ' ')) {
        commandsParsed.push_back(tmp);
    }
    // TODO Need to fix parsing
    for (size_t i = 0; i < commandsParsed.size(); i++) {
        if (i == 0 && std::find(commandsAvaliable.begin(),
                                commandsAvaliable.end(), commandsParsed[i]) != commandsAvaliable.end()) {
            std::cout << commandsParsed[i] << " ";
        } else
            std::cout << commandsParsed[i] << " ";
    }


}

std::vector<std::string> User::getCommandsParsed() {
    return this->commandsParsed;
}