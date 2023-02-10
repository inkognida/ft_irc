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

    if (commandsParse.size() == 3 && commandsParse[2][0] == ':') {
        Channels[commandsParse[1]].clearTopic();
        backMSG(user, RPL_NOTOPIC, user.getCmd());
        return ;
    }

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

void            Server::MODE(User &user) {
    if (commandsParse.size() == 1 || commandsParse.size() == 2) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (Channels.find(commandsParse[1]) == Channels.end()) {
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    if (commandsParse[2] == "+t")
        Channels[commandsParse[1]].addMode("+t");
    else if (commandsParse[2] == "+n")
        Channels[commandsParse[1]].addMode("+n");
    else if (commandsParse[2] == "+m")
        Channels[commandsParse[1]].addMode("+m");
    else if (commandsParse[2] == "+v") {
        if (commandsParse[3].empty()) {
            backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
            return ;
        }

        for (size_t i = 0; i < Users.size(); i++) {
            if (Users[i].getNickname() == commandsParse[3] && Channels[commandsParse[1]].findUser(Users[i])
                && Channels[commandsParse[1]].findOper(user)) {
                Channels[commandsParse[1]].addMode("+v");
                Users[i].setMode("+v");
                Users[i].setBackMSG(SERVER + std::to_string(RPL_VOICEPRIVILEGE) + " = " +
                    Channels[commandsParse[1]].getName() + " : Now you have voice privilege");
                user.setBackMSG(SERVER + std::to_string(RPL_VOICEPRIVILEGE) + " = " +
                    Channels[commandsParse[1]].getName() + " : You gave voice privilege to " + Users[i].getNickname());
                return ;
            }
        }
        backMSG(user, ERR_NOSUCHNICK, user.getCmd());
        return ;
    } else if (commandsParse[2] == "+i")
        Channels[commandsParse[1]].addMode("+i");
    else {
        backMSG(user, ERR_UNKNOWNMODE, user.getCmd());
        return ;
    }

    user.setBackMSG(SERVER + std::to_string(RPL_CHANNELMODEIS) + " = " +
        Channels[commandsParse[1]].getName() + " channel mode: " + commandsParse[2]);
}

void            Server::KICK(User &user, std::string content) {
    content = content.substr(commandsParse[0].size() + commandsParse[1].size() + //TODO check this substr output with reason
                commandsParse[2].size() + 3, content.size());
    if (commandsParse.size() < 3) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (Channels.find(commandsParse[1]) == Channels.end()) {
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    for (size_t i = 0; i < Users.size(); i++) {
        if (Users[i].getNickname() == commandsParse[2] && Channels[commandsParse[1]].findUser(Users[i])
                && Channels[commandsParse[1]].findOper(user)) {
            Channels[commandsParse[1]].deleteUser(Users[i]);
            Users[i].setBackMSG(SERVER + std::to_string(RPL_USERKICKED) + " = You have been kicked from channel: " +
                Channels[commandsParse[1]].getName() + " : " + content);
            user.setBackMSG(SERVER + std::to_string(RPL_USERKICKED) + " = You kicked user: " +
                Users[i].getNickname() + " from channel: " + commandsParse[1] + " : " + content);
            return ;
        }
    }
    backMSG(user, ERR_NOSUCHNICK, user.getCmd());
    return ;
}

int            Server::userExists(std::string targetNick) {
    for (size_t i = 0; i < Users.size(); i++) {
        if (Users[i].getNickname() == targetNick) {
            return i;
        }
    }
    return -1;
}

void            Server::INVITE(User &user) {
    if (commandsParse.size() != 3) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    int target = userExists(commandsParse[1]);

    if (!target) {
        backMSG(user, ERR_NOSUCHNICK, user.getCmd());
        return ;
    }


    if (Channels.find(commandsParse[2]) != Channels.end()) {
        if (!(Channels[commandsParse[1]].findUser(user) || Channels[commandsParse[1]].findOper(user)) &&
        (Channels[commandsParse[1]].findUser(Users[target]) || Channels[commandsParse[1]].findOper(Users[target]))) {
            backMSG(user, ERR_USERONCHANNEL, user.getCmd());
            return ;
        }
    } else {
        backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        return ;
    }

    if (Channels[commandsParse[2]].findMode("+i")) {
        if (Channels[commandsParse[2]].findOper(user)) {
            Users[target].setMode("+i");
            Users[target].setBackMSG(SERVER + std::to_string(RPL_INVITING) + " You was invited to channel " +
                Channels[commandsParse[2]].getName() + " by " + user.getNickname());
            user.setBackMSG(SERVER + std::to_string(RPL_INVITING) + " You invited to " +
                Channels[commandsParse[2]].getName() + " target: " + Users[target].getNickname());
        } else
            backMSG(user, ERR_CHANOPRIVSNEEDED, user.getCmd());

    } else {
        if ((Channels[commandsParse[2]].findUser(user) || Channels[commandsParse[2]].findOper(user))) {
            Users[target].setBackMSG(SERVER + std::to_string(RPL_INVITING) + " You was invited to channel " +
                Channels[commandsParse[2]].getName() + " by " + user.getNickname());
            user.setBackMSG(SERVER + std::to_string(RPL_INVITING) + " You invited to " +
                Channels[commandsParse[2]].getName() + " target: " + Users[target].getNickname());
        } else
            backMSG(user, ERR_NOTONCHANNEL, user.getCmd());
    }
}

