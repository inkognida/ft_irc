#include "../headers/irc.hpp"

void    Server::PASS(User &user) {
    if (commandsParse.size() != 2)
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
    else if (!user.getNewUser()) {
        backMSG(user, ERR_ALREADYREGISTERED, user.getCmd());
    }
    else if (commandsParse[1] != password)
        backMSG(user, ERR_PASSWDMISMATCH, user.getCmd());
    else
        user.setPassword(password);
}

static bool correctNickname(std::string nick) {
    if (nick.size() > 20)
        return false;

    for (size_t i = 0; i < nick.size(); i++) {
        if (!isalpha(nick[i]) && !isdigit(nick[i]) && nick[i] != '_')
            return false;
    }

    return true;
}

void    Server::NICK(User &user) {
    if (commandsParse.size() != 2) {
        backMSG(user, ERR_NONICKNAMEGIVEN, user.getCmd());
        return ;
    }

    if (!correctNickname(commandsParse[1])) {
        backMSG(user, ERR_ERRONEUSNICKNAME, user.getCmd());
        return ;
    }

    if (userExists(commandsParse[1]) >= 0) {
        backMSG(user, ERR_NICKNAMEINUSE, user.getCmd());
        return ;
    }

    if (!user.getNickname().empty())
        user.setBackMSG(SERVER + std::string("user: ") + user.getNickname() + " changed nickname to " + commandsParse[1]);

    user.setNickname(commandsParse[1]);
}

static bool correctUserInfo(std::string arg, int pos) {
    if (arg.size() > 20 && pos != 4)
        return false;

    if (pos == 1 || pos == 3) {
        for (size_t i = 0; i < arg.size(); i++) {
            if (!isalpha(arg[i]) && arg[i] != '_')
                return false;
        }
    }

    if (pos == 2) {
        for (size_t i = 0; i < arg.size(); i++) {
            if (!isalpha(arg[i]) && !isdigit(arg[i]))
                return false;
        }
    }

    if (pos == 4) {
        for (size_t i = 0; i < arg.size(); i++) {
            if (arg[i] == '#')
                return false;
        }
    }

    return true;
}

int Server::userInfoExists(std::string usermode) {
    std::map<int, User>::const_iterator begin = Users.begin();
    std::map<int, User>::const_iterator end = Users.end();

    while (begin != end) {
        if (begin->second.getUser() == usermode)
            return begin->second.getSocket();
        begin++;
    }

    return -1;
}

void    Server::USER(User &user, std::string content) {
    if (commandsParse.size() < 5) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (!user.getNewUser()) {
        backMSG(user, ERR_ALREADYREGISTERED, user.getCmd());
        return ;
    }

    content = content.substr(commandsParse[0].size() + commandsParse[1].size() + commandsParse[2].size()
            + commandsParse[3].size() + 4, content.size());

    if (!correctUserInfo(commandsParse[1], 1) || !correctUserInfo(commandsParse[2], 2) ||
        !correctUserInfo(commandsParse[3], 3) || !correctUserInfo(content, 4)) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    if (userInfoExists(commandsParse[1]) >= 0) {
        backMSG(user, ERR_ALREADYREGISTERED, user.getCmd());
        return ;
    }

    user.setUser(commandsParse[1]);
    user.setUserMode(commandsParse[2]);
    user.setUnused(commandsParse[3]);
    user.setRealname(content);
}
