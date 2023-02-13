#include "../headers/irc.hpp"

void    Server::channelPRIVMSG(User &user, std::string content) {
    if (Channels[commandsParse[1]].findMode("+m")) {
        if (Channels[commandsParse[1]].findOper(user) || user.checkChannelMode(commandsParse[1], "+v")) {
            Channels[commandsParse[1]].sendNotificationMsg(Users, content, user);
        } else
            backMSG(user, ERR_CANNOTSENDTOCHAN, user.getCmd());
    } else if (Channels[commandsParse[1]].findMode("+n")) {
        if (Channels[commandsParse[1]].findUser(user) || Channels[commandsParse[1]].findOper(user))
            Channels[commandsParse[1]].sendNotificationMsg(Users, content, user);
        else
            backMSG(user, ERR_CANNOTSENDTOCHAN, user.getCmd());
    } else
        Channels[commandsParse[1]].sendNotificationMsg(Users, content, user);
}

void    Server::PRIVMSG(User &user, std::string content) {
    if (commandsParse.size() == 1) {
        backMSG(user, ERR_NORECIPIENT, user.getCmd());
        return ;
    }

    if (commandsParse.size() == 2) {
        backMSG(user, ERR_NOTEXTTOSEND, user.getCmd());
        return ;
    }

    content = content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size());

    if (Channels.find(commandsParse[1]) != Channels.end()) {
        channelPRIVMSG(user, content);
        return;
    } else if (commandsParse[1][0] == '#') {
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    int target = userExists(commandsParse[1]);

    if (target < 0) {
        backMSG(user, ERR_NOSUCHNICK, user.getCmd());
        return ;
    }

    Users[target].setBackMSG(SERVER + std::string("= ") + user.getNickname() + ": " + content);
}

void    Server::channelNOTICE(User &user, std::string content) {
    if (Channels[commandsParse[1]].findMode("+m")) {
        if (Channels[commandsParse[1]].findOper(user) || user.checkChannelMode(commandsParse[1], "+v")) {
            Channels[commandsParse[1]].sendNotificationMsg(Users, content, user);
        } else
            return ;
    } else if (Channels[commandsParse[1]].findMode("+n")) {
        if (Channels[commandsParse[1]].findUser(user) || Channels[commandsParse[1]].findOper(user))
            Channels[commandsParse[1]].sendNotificationMsg(Users, content, user);
        else
            return ;
    } else
        Channels[commandsParse[1]].sendNotificationMsg(Users, content, user);
}

void    Server::NOTICE(User &user, std::string content) {
    if (commandsParse.size() <= 2)
        return ;

    content = content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size());

    if (Channels.find(commandsParse[1]) != Channels.end()) {
        channelNOTICE(user, content);
        return ;
    }
    else if (commandsParse[1][0] == '#')
        return ;

    int target = userExists(commandsParse[1]);

    if (target < 0)
        return ;

    Users[target].setBackMSG(SERVER + std::string("= ") + user.getNickname() + ": " + content);
    return ;
}
