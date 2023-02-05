#include "../headers/irc.hpp"


static std::vector<std::string> splitChannelArgs(std::string &names) {
    std::vector<std::string> res;
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
        if (!isalpha(name[i]) || !isdigit(name[i]) || name[i] == ',')
            return false;
    }
    return true;
}

void    Server::JOIN(User &user) {
    if (commandsParse.size() != 3) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    std::vector<std::string> names = splitChannelArgs(commandsParse[1]);
    std::vector<std::string> pswds = splitChannelArgs(commandsParse[2]);

    for (size_t i = 0; i < names.size(); i++) {
        // new channel
        if (correctName(names[i]) && Channels.find(names[i]) == names.end()) {
            if (i < pswds.size()) {
                Channels.insert(std::pair<std::string, Channel>(names[i], Channel(names[i], pswds[i])));
                Channels[names[i]].addOperator(user.getSocket());
                user.setBackMSG(SERVER + std::to_string(RPL_CREATECHANNEL) + " = " + Channels[names[i]].getName() +
                    Channels[names[i]].getUsers(Users) + Channels[names[i]].getOperators(Users) + "\n");
            }
            else
                continue;
        }
        // channel exists
        if (correctName(names[i]) && Channels.find(names[i]) != names.end()) {
            continue ;
        }
    }


}