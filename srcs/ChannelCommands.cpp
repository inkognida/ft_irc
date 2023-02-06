#include "../headers/irc.hpp"

void        Server::TOPIC(User &user, std::string content) {
    if (commandsParse.size() == 1) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (commandsParse.size() == 2) {
        if (!Channels.find(commandsParse[1])->second.getTopic().empty())
            Channels[commandsParse[1]].sendNotificationTopic(user);
        else
            backMSG(user, RPL_NOTOPIC, user.getCmd());
        return ;
    }

    if (commandsParse.size() == 3 && commandsParse[2][0] == ':') {
        Channels[commandsParse[1]].clearTopic();
        backMSG(user, RPL_NOTOPIC, user.getCmd());
        return ;
    }

    std::cout << Channels[commandsParse[1]].findOper(user) << std::endl;

    content = content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size());
    if (Channels[commandsParse[1]].findOper(user) || Channels[commandsParse[1]].findUser(user)) {
        if (Channels[commandsParse[1]].findMode("+t") && Channels[commandsParse[1]].findOper(user)) {
            Channels[commandsParse[1]].setTopic(content);
            Channels[commandsParse[1]].sendNotificationTopic(user);
        }
        else if (Channels[commandsParse[1]].findMode("+t") && !Channels[commandsParse[1]].findOper(user))
            backMSG(user, ERR_CHANOPRIVSNEEDED, user.getCmd());
        else {
            Channels[commandsParse[1]].setTopic(content);
            Channels[commandsParse[1]].sendNotificationTopic(user);
        }
        return ;
    } else
        backMSG(user, ERR_NOTONCHANNEL, user.getCmd());
}