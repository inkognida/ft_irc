#include "../headers/User.hpp"

User::User() {
    commands["PASS"] = 0;
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

void User::setCommandsToParse(std::string content) {
    char *words = new char [content.length()+1];
    std::strcpy(words, content.c_str());
    char *line = strtok(words, " ");

    while(line != NULL)
    {
        commandsParsed.push_back(line);
        line = strtok(NULL, "\r \n");
    }

    for (size_t i = 0; i < commandsParsed.size(); i++)
        std::cout <<

    commandsParsed.clear();
}

std::vector<std::string> User::getCommandsParsed() {
    return this->commandsParsed;
}


int		User::readMessage()
{
    std::string	text;
    if (messages.size() > 0)
        text = messages.front();
    char buffer[100];
    int bytesRead;
    while ((bytesRead = recv(sockfd, buffer, 99, 0)) > 0)
    {
        buffer[bytesRead] = 0;
        text += buffer;
        buffer[0] = 0;
        if (text.find('\n') != std::string::npos)
            break;
    }
    if (text.length() > 512)
        text = text.substr(0, 510) + "\r\n";
    if (bytesRead == 0)
        return (DISCONNECT);
    while (text.find("\r\n") != std::string::npos)
        text.replace(text.find("\r\n"), 2, "\n");
    if (text.size() > 1)
        messages = split(text, '\n', true);
    return 0;
}
