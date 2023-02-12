#include "../headers/irc.hpp"

static std::vector<std::string> splitChannelArgs(std::string &names) {
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

static bool correctName(std::string name) {
    if (name.size() > 50 || name[0] != '#')
        return false;
    for (size_t i = 1; i < name.size(); i++) {
        if (!isalpha(name[i]) && !isdigit(name[i]) && name[i] == ',')
            return false;
    }
    return true;
}

void    Server::channelJOIN(User &user, std::string name, bool create, std::string passwd = "") {
    // new channel creation
    if (create && passwd.empty()) {
        Channels.insert(std::pair<std::string, Channel>(name, Channel(name)));
        Channels[name].addOperator(user);
        Channels[name].sendNotificationJoin(Users);
        user.addChannel(name);
    } else if (create) {
        Channels.insert(std::pair<std::string, Channel>(name, Channel(name, passwd)));
        Channels[name].addOperator(user);
        Channels[name].sendNotificationJoin(Users);
        user.addChannel(name);
    }

    // channel exists
    if (!create && (Channels[name].findOper(user) || Channels[name].findUser(user)))
        backMSG(user, ERR_CHANNELISFULL, user.getCmd());
    else if (Channels[name].findMode("+i") && user.inviteExists(name)) {
        Channels[name].addUser(user);
        Channels[name].sendNotificationJoin(Users);
        user.deleteInvite(name);
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

// TODO test it more -> maybe u should add some logic
void    Server::JOIN(User &user) {
    if (commandsParse.size() != 2 && commandsParse.size() != 3) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
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

// before
/*
 *     if (commandsParse.size() == 3)
        pswds = splitChannelArgs(commandsParse[2]);


    for (size_t i = 0; i < names.size(); i++) {
        if (!correctName(names[i]))
            backMSG(user, ERR_BADCHANMASK, user.getCmd());
        // new channel
        else if (Channels.find(names[i]) == Channels.end()) {
            if (i < pswds.size()) {
                Channels.insert(std::pair<std::string, Channel>(names[i], Channel(names[i], pswds[i])));
                Channels[names[i]].addOperator(user);
                Channels[names[i]].sendNotificationJoin(Users);
                user.addChannel(names[i]);
            }
            else {
                Channels.insert(std::pair<std::string, Channel>(names[i], Channel(names[i])));
                Channels[names[i]].addOperator(user);
                Channels[names[i]].sendNotificationJoin(Users);
                user.addChannel(names[i]);
            }
        }
        // channel exists
        else if (Channels[names[i]].findOper(user) || Channels[names[i]].findUser(user))
            backMSG(user, ERR_CHANNELISFULL, user.getCmd());
        else {
            //check this section
            if (Channels[names[i]].findMode("+i") && user.checkMode("+i")) {
                Channels[names[i]].addUser(user);
                Channels[names[i]].sendNotificationJoin(Users);
                user.deleteMode("+i");
                user.addChannel(names[i]);
            } else if (Channels[names[i]].findMode("+i")) {
                backMSG(user, ERR_INVITEONLYCHAN, user.getCmd());
                continue ;
            }

            if (i < pswds.size() && Channels[names[i]].getPass() == pswds[i]) {
                Channels[names[i]].addUser(user);
                Channels[names[i]].sendNotificationJoin(Users);
                user.addChannel(names[i]);
            } else if ((i < pswds.size() && Channels[names[i]].getPass() != pswds[i]) ||
                    (pswds.size() == 0 && !Channels[names[i]].getPass().empty()))
                backMSG(user, ERR_BADCHANNELKEY, user.getCmd());
            else {
                Channels[names[i]].addUser(user);
                Channels[names[i]].sendNotificationJoin(Users);
                user.addChannel(names[i]);
            }
        }
    }
 */