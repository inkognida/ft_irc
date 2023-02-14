#include "../headers/irc.hpp"

std::vector<std::string> splitChannelArgs(std::string &names) {
    std::vector<std::string> res;

    if (names.empty())
        return res;

    char *tmp = std::strtok(const_cast<char*>(names.c_str()), ",");

    while (tmp != nullptr) {
        res.push_back(std::string(tmp));
        tmp = std::strtok(NULL, ",");
    }

    return res;
}

bool correctName(std::string name) {
    if (name.size() > 50 || name[0] != '#')
        return false;
    for (size_t i = 1; i < name.size(); i++) {
        if (!isalpha(name[i]) && !isdigit(name[i]) && name[i] == ',')
            return false;
    }
    return true;
}

void                       Server::outAllUsersCmd(User &user, std::string cmd)  {
    std::map<int, User>::const_iterator begin = Users.begin();
    std::map<int, User>::const_iterator end = Users.end();

    while (begin != end) {
        if (cmd == "NAMES")
            user.setBackMSG(SERVER + std::to_string(RPL_NAMREPLY) + " " + begin->second.getNickname());
        begin++;
    }
}

void                       Server::outAllChannelsCmd(User &user, std::string cmd)  {
    std::map<std::string, Channel>::const_iterator begin = Channels.begin();
    std::map<std::string, Channel>::const_iterator end = Channels.end();

    while (begin != end) {
        if (cmd == "LIST")
            user.setBackMSG(SERVER + std::to_string(RPL_LIST) + " " + begin->first + " topic: " +
                            begin->second.getTopic());
        else if (cmd == "NAMES")
            user.setBackMSG(SERVER + std::to_string(RPL_NAMREPLY) + " " + begin->first);
        begin++;
    }
}

void                        Server::outCurrentChannelsCmd(User &user, std::string cmd)  {
    std::vector<std::string> names = splitChannelArgs(commandsParse[1]);

    for (size_t i = 0; i < names.size(); i++) {
        if (!correctName(names[i]))
            backMSG(user, ERR_BADCHANMASK, user.getCmd());
        else if (Channels.find(names[i]) == Channels.end())
            backMSG(user, ERR_NOSUCHCHANNEL, user.getCmd());
        else if (cmd == "LIST")
            user.setBackMSG(SERVER + std::to_string(RPL_LIST) + " " + names[i] + " topic: " +
                            Channels[names[i]].getTopic());
        else if (cmd == "NAMES")
            user.setBackMSG(SERVER + std::to_string(RPL_NAMREPLY) + " " + names[i] + " users: " +
                            Channels[names[i]].getUsersInfo(Users) + Channels[names[i]].getOperatorsInfo(Users));
        else if (cmd == "LIST users")
            user.setBackMSG(SERVER + std::to_string(RPL_LIST) + " " + names[i] + " users: " +
                            Channels[names[i]].getUsersInfo(Users) + Channels[names[i]].getOperatorsInfo(Users));
    }
}

void    Server::LIST(User &user) {
    if (commandsParse.size() < 1 || commandsParse.size() > 3) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (commandsParse.size() == 3 && commandsParse[2] == "users") {
        outCurrentChannelsCmd(user, "LIST users");
        return ;
    }

    if (commandsParse.size() == 1) {
        outAllChannelsCmd(user, "LIST");
        return ;
    }

    outCurrentChannelsCmd(user, "LIST");
}


void    Server::NAMES(User &user) {
    if (commandsParse.size() < 1 || commandsParse.size() > 2) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (commandsParse.size() == 1) {
        outAllChannelsCmd(user, "NAMES");
        outAllUsersCmd(user, "NAMES");
        return ;
    }

    outCurrentChannelsCmd(user, "NAMES");
}

void    Server::channelJOIN(User &user, std::string name, bool create, std::string passwd = "") {
    // new channel creation
    if (create && passwd.empty()) {
        Channels.insert(std::pair<std::string, Channel>(name, Channel(name)));
        Channels[name].addOperator(user);
        Channels[name].sendNotificationJoin(Users);
        user.addChannel(name);
        user.addChannelMode(name, "+o");
    } else if (create) {
        Channels.insert(std::pair<std::string, Channel>(name, Channel(name, passwd)));
        Channels[name].addOperator(user);
        Channels[name].sendNotificationJoin(Users);
        user.addChannel(name);
        user.addChannelMode(name, "+o");
    }

    // channel exists
    if (!create && (Channels[name].findOper(user) || Channels[name].findUser(user)))
        backMSG(user, ERR_CHANNELISFULL, user.getCmd());
    else if (Channels[name].findMode("+i") && user.checkChannelMode(name, "+i")) {
        Channels[name].addUser(user);
        Channels[name].sendNotificationJoin(Users);
        user.addChannel(name);
    }
    else if (Channels[name].findMode("+i"))
        backMSG(user, ERR_INVITEONLYCHAN, user.getCmd());
    else if (!create && passwd.empty()) {
        if (!Channels[name].getPass().empty())
            backMSG(user, ERR_BADCHANNELKEY, user.getCmd());
        else {
            Channels[name].addUser(user);
            Channels[name].sendNotificationJoin(Users);
            user.addChannel(name);
        }
    }
    else if (!create) {
        if (Channels[name].getPass() != passwd) {
            backMSG(user, ERR_BADCHANNELKEY, user.getCmd());
        } else {
            Channels[name].addUser(user);
            Channels[name].sendNotificationJoin(Users);
            user.addChannel(name);
        }
    }
}

void    Server::JOIN(User &user) {
    if (commandsParse.size() != 2 && commandsParse.size() != 3) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    // leave all joined channels
    if (commandsParse.size() == 2 && commandsParse[1] == "0") {
        user.setBackMSG(SERVER + std::to_string(RPL_LEFTALLCHANNELS) + " You left all joined channels");
        user.showLeftChannels();
        user.quitChannels(Channels);
        return ;
    }

    std::vector<std::string> names = splitChannelArgs(commandsParse[1]);
    if (commandsParse.size() == 2) {
        for (size_t i = 0; i < names.size(); i++) {
            if (!correctName(names[i]))
                backMSG(user, ERR_BADCHANMASK, user.getCmd());
            //new channel
            else if (Channels.find(names[i]) == Channels.end())
                channelJOIN(user, names[i], true);
            //user in channel
            else if (Channels[names[i]].findUser(user) || Channels[names[i]].findOper(user))
                backMSG(user, ERR_CHANNELISFULL, user.getCmd());
            //channel exists
            else
                channelJOIN(user, names[i], false);
        }
    }

    if (commandsParse.size() == 3) {
        std::vector<std::string> pswds = splitChannelArgs(commandsParse[2]);

        if (pswds.size() != names.size()) {
            backMSG(user, ERR_CHANNELISFULL, user.getCmd());
            return ;
        }
        for (size_t i = 0; i < names.size(); i++) {
            if (!correctName(names[i]))
                backMSG(user, ERR_BADCHANMASK, user.getCmd());
            //new channel
            else if (Channels.find(names[i]) == Channels.end())
                channelJOIN(user, names[i], true, pswds[i]);
            //user in channel
            else if (Channels[names[i]].findOper(user) || Channels[names[i]].findUser(user))
                backMSG(user, ERR_CHANNELISFULL, user.getCmd());
            //channel exists
            else
                channelJOIN(user, names[i], false, pswds[i]);
        }
    }
}
