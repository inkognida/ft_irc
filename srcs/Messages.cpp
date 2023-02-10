#include "../headers/irc.hpp"

void    Server::channelPRIVMSG(User &user, std::string content) {
    if (Channels[commandsParse[1]].findMode("+m")) {
        if (Channels[commandsParse[1]].findOper(user) || user.checkMode("+v")) {
            Channels[commandsParse[1]].sendNotificationPrivmsg(Users, content, user);
        } else
            backMSG(user, ERR_CANNOTSENDTOCHAN, user.getCmd());
    } else if (Channels[commandsParse[1]].findMode("+n")) {
        if (Channels[commandsParse[1]].findUser(user) || Channels[commandsParse[1]].findOper(user))
            Channels[commandsParse[1]].sendNotificationPrivmsg(Users, content, user);
        else
            backMSG(user, ERR_CANNOTSENDTOCHAN, user.getCmd());
    } else
        Channels[commandsParse[1]].sendNotificationPrivmsg(Users, content, user);
}

void    Server::PRIVMSG(User &user, std::string content) {
    if (commandsParse[1].empty()) {
        backMSG(user, ERR_NORECIPIENT, user.getCmd());
        return ;
    }

    if (commandsParse[2].empty()) {
        backMSG(user, ERR_NOTEXTTOSEND, user.getCmd());
        return ;
    }

    content = content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size());

    if (Channels.find(commandsParse[1]) != Channels.end()) {
        channelPRIVMSG(user, content);
        return;
    } else if (Channels.find(commandsParse[1]) == Channels.end()) {
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    std::map<int, User>::iterator begin = Users.begin();
    std::map<int, User>::iterator end = Users.end();

    while (begin != end) {
        if (begin->second.getNickname() == commandsParse[1]) {
            begin->second.setBackMSG(content);
            user.setBackMSG(content);
            return ;
        }
        begin++;
    }

    backMSG(user, ERR_NOSUCHNICK, user.getCmd());
    return ;
}
