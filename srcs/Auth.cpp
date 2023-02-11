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
    else
        user.clearBackMSG();
    user.setNickname(commandsParse[1]);
}

int     Server::correctUSER(std::string user, int pos, User &user_) {
    if (pos == 1 || pos == 4) {
        for (size_t i = 0; i < user.size(); i++) {
            if (!isalpha(user[i]) && user[i] != '_')
                return 1;
        }
    }

    if (pos == 2 || pos == 3) {
        for (size_t i = 0; i < user.size(); i++) {
            if (!isalpha(user[i]) && !isdigit(user[i]))
                return 1;
        }
    }

    if (pos == 1) {
        std::map<int, User>::iterator begin = Users.begin();
        std::map<int, User>::iterator end = Users.end();

        while (begin != end) {
            if (begin->second.getUser() == user && begin->second.getNickname() != user_.getNickname())
                return 2;
            begin++;
        }
    }

    return 3;
}

void    Server::USER(User &user) {
    if (commandsParse.size() != 5) {
        backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
        return ;
    }

    for (size_t i = 1; i < commandsParse.size(); i++) {
        switch (correctUSER(commandsParse[i], i, user)) {
            case 1:
                backMSG(user, ERR_ALREADYREGISTERED, user.getCmd());
                return ;
            case 2:
                backMSG(user, ERR_NEEDMOREPARAMS, user.getCmd());
                return ;
            case 3:
                user.clearBackMSG();
                if (i == 1)
                    user.setUser(commandsParse[i]);
                if (i == 2)
                    user.setUserMode(commandsParse[i]);
                if (i == 3)
                    user.setUnused(commandsParse[i]);
                if (i == 4)
                    user.setRealname(commandsParse[i]);
        }
    }
}