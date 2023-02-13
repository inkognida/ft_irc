#include "../headers/irc.hpp"

void        Server::TOPIC(User &user, std::string content) {
    if (commandsParse.size() == 1) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (Channels.find(commandsParse[1]) == Channels.end()) {
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    if (commandsParse.size() == 2) {
        if (!Channels.find(commandsParse[1])->second.getTopic().empty())
            Channels[commandsParse[1]].sendNotificationTopic(user);
        else
            backMSG(user, RPL_NOTOPIC, user.getCmd());
        return ;
    }

    if (commandsParse[2][0] == ':' && commandsParse[2].size() == 1) {
        if (Channels[commandsParse[1]].findOper(user)) {
            Channels[commandsParse[1]].clearTopic();
            backMSG(user, RPL_NOTOPIC, user.getCmd());
        } else
            backMSG(user, ERR_CHANOPRIVSNEEDED, user.getCmd());
        return ;
    }

    content = content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size());
    if (Channels[commandsParse[1]].findOper(user) || Channels[commandsParse[1]].findUser(user)) {
        if (Channels[commandsParse[1]].findMode("+t") && Channels[commandsParse[1]].findOper(user)) {
            Channels[commandsParse[1]].setTopic(content);
            Channels[commandsParse[1]].sendNotificationTopic(user);
        }
        else if (Channels[commandsParse[1]].findMode("+t"))
            backMSG(user, ERR_CHANOPRIVSNEEDED, user.getCmd());
        else {
            Channels[commandsParse[1]].setTopic(content);
            Channels[commandsParse[1]].sendNotificationTopic(user);
        }
        return ;
    } else
        backMSG(user, ERR_NOTONCHANNEL, user.getCmd());
}

void            Server::userMODE(User &user, std::string mode) {
    if (commandsParse[3].empty()) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    int target = userExists(commandsParse[3]);

    if (target < 0) {
        backMSG(user, ERR_NOSUCHNICK, user.getCmd());
        return ;
    }

    if (!Channels[commandsParse[1]].findUser(Users[target])) {
        backMSG(user, ERR_USERNOTINCHANNEL, user.getCmd());
        return ;
    }

    if (!Channels[commandsParse[1]].findOper(user)) {
        backMSG(user, ERR_CHANOPRIVSNEEDED, user.getCmd());
        return ;
    }

    if (mode == "+v") {
        Users[target].addChannelMode(commandsParse[1], "+v");
        Users[target].setBackMSG(SERVER + std::to_string(RPL_VOICEPRIVILEGE) + " = " +
                                 commandsParse[1] + " : Now you have voice privilege");
        user.setBackMSG(SERVER + std::to_string(RPL_VOICEPRIVILEGE) + " = " +
                        commandsParse[1] + " : You gave voice privilege to " + Users[target].getNickname());
        return ;
    } else if (mode == "-v") {
        Users[target].deleteChannelMode(commandsParse[1], "+v");
        Users[target].setBackMSG(SERVER + std::to_string(RPL_VOICEPRIVILEGE) + " = " +
                                 commandsParse[1] + " : Now you don't have voice privilege");
        user.setBackMSG(SERVER + std::to_string(RPL_VOICEPRIVILEGE) + " = " +
                        commandsParse[1] + " : You take voice privilege from " + Users[target].getNickname());
        return ;
    }
}

void            Server::MODE(User &user) {
    if (commandsParse.size() == 1 || commandsParse.size() == 2) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (Channels.find(commandsParse[1]) == Channels.end()) {
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    if (!Channels[commandsParse[1]].findOper(user)) {
        backMSG(user, ERR_CHANOPRIVSNEEDED, user.getCmd());
        return ;
    }

    if (commandsParse[2] == "+t")
        Channels[commandsParse[1]].addMode("+t");
    else if (commandsParse[2] == "-t")
        Channels[commandsParse[1]].deleteMode("+t");

    else if (commandsParse[2] == "+n")
        Channels[commandsParse[1]].addMode("+n");
    else if (commandsParse[2] == "-n")
        Channels[commandsParse[1]].deleteMode("+n");

    else if (commandsParse[2] == "+m")
        Channels[commandsParse[1]].addMode("+m");
    else if (commandsParse[2] == "-m")
        Channels[commandsParse[1]].deleteMode("+m");

    else if (commandsParse[2] == "+i")
        Channels[commandsParse[1]].addMode("+i");
    else if (commandsParse[2] == "-i")
        Channels[commandsParse[1]].deleteMode("+i");

    // user modes
    else if (commandsParse[2] == "+v" || commandsParse[2] == "-v") {
        userMODE(user, commandsParse[2]);
        return ;
    }
    else {
        backMSG(user, ERR_UNKNOWNMODE, user.getCmd());
        return ;
    }

    user.setBackMSG(SERVER + std::to_string(RPL_CHANNELMODEIS) + " = " +
        Channels[commandsParse[1]].getName() + " channel mode: " + commandsParse[2]);
}

void            Server::KICK(User &user, std::string content) {
    if (commandsParse.size() < 3) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (Channels.find(commandsParse[1]) == Channels.end()) {
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    if (commandsParse.size() == 3)
        content = content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size());
    else
        content = content.substr(commandsParse[0].size() + commandsParse[1].size() +
                    commandsParse[2].size() + 3, content.size());

    int target = userExists(commandsParse[2]);

    if (target < 0)
        backMSG(user, ERR_NOSUCHNICK, user.getCmd());
    else if (target == user.getSocket())
        backMSG(user, ERR_BADCHANMASK, user.getCmd());
    else if (!Channels[commandsParse[1]].findUser(Users[target]))
        backMSG(user, ERR_USERNOTINCHANNEL, user.getCmd());
    else if (!Channels[commandsParse[1]].findOper(user))
        backMSG(user, ERR_NOTONCHANNEL, user.getCmd());
    else {
        Users[target].setBackMSG(SERVER + std::to_string(RPL_USERKICKED) +
                                 " = You have been kicked from channel: " + commandsParse[1] + " : " + content);
        user.setBackMSG(SERVER + std::to_string(RPL_USERKICKED) + " = You kicked user: " +
                        Users[target].getNickname() + " from channel: " + commandsParse[1] + " : " + content);

        Channels[commandsParse[1]].deleteUser(Users[target]);
    }
}

int            Server::userExists(std::string targetNick) {
    std::map<int, User>::const_iterator begin = Users.begin();
    std::map<int, User>::const_iterator end = Users.end();

    while (begin != end) {
        if (begin->second.getNickname() == targetNick)
            return begin->second.getSocket();
        begin++;
    }

    return -1;
}

void            Server::INVITE(User &user) {
    if (commandsParse.size() != 3) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    int target = userExists(commandsParse[1]);

    if (target < 0) {
        backMSG(user, ERR_NOSUCHNICK, user.getCmd());
        return ;
    }

    if (Channels.find(commandsParse[2]) != Channels.end() &&
        (Channels[commandsParse[2]].findUser(Users[target]) ||
        Channels[commandsParse[2]].findOper(Users[target]))) {
        backMSG(user, ERR_USERONCHANNEL, user.getCmd());
        return ;
    } else if (Channels.find(commandsParse[2]) == Channels.end()){
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    if (Channels[commandsParse[2]].findMode("+i")) {
        if (Channels[commandsParse[2]].findOper(user)) {
            Users[target].addChannelMode(commandsParse[2], "+i");
            Users[target].setBackMSG(SERVER + std::to_string(RPL_INVITING) + " You was invited to channel " +
                commandsParse[2] + " by " + user.getNickname());
            user.setBackMSG(SERVER + std::to_string(RPL_INVITING) + " You invited to " +
                commandsParse[2] + " target: " + Users[target].getNickname());
        } else
            backMSG(user, ERR_CHANOPRIVSNEEDED, user.getCmd());
    } else {
        if ((Channels[commandsParse[2]].findUser(user) || Channels[commandsParse[2]].findOper(user))) {
            Users[target].setBackMSG(SERVER + std::to_string(RPL_INVITING) + " You was invited to channel " +
                commandsParse[2] + " by " + user.getNickname());
            user.setBackMSG(SERVER + std::to_string(RPL_INVITING) + " You invited to " +
                commandsParse[2] + " target: " + Users[target].getNickname());
        } else
            backMSG(user, ERR_NOTONCHANNEL, user.getCmd());
    }
}

