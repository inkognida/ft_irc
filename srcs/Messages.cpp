#include "../headers/irc.hpp"

void    Server::PRIVMSG(User &user, std::string content) {
    //TODO channels
    if (commandsParse[1].empty()) {
        backMSG(user, ERR_NORECIPIENT, user.getCmd());
        return ;
    }
    if (commandsParse[2].empty()) {
        backMSG(user, ERR_NOTEXTTOSEND, user.getCmd());
        return ;
    }

    std::map<int, User>::iterator begin = Users.begin();
    std::map<int, User>::iterator end = Users.end();


    while (begin != end) {
        if (begin->second.getNickname() == commandsParse[1]) {
            begin->second.setBackMSG(content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size()) + "\n");
            user.setBackMSG(content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size()) + "\n");
            return ;
        }
        begin++;
    }

    backMSG(user, ERR_NORECIPIENT, user.getCmd());
    return ;
}
