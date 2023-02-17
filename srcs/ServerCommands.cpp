#include "../headers/irc.hpp"

// IMITATION OF PING
void        Server::PING(User &user, std::string content) {
    if (commandsParse.size() < 2) {
        backMSG(user, ERR_NOORIGIN, user.getCmd());
        return ;
    }
    content = content.substr(commandsParse[0].size() + 1, content.size());
    user.setBackMSG(SERVER + std::string("PONG ") + content);
}

// BONUS
void        Server::fileTransfer(User &user) {
    std::ifstream file("test.txt", std::ifstream::binary);
    if (!file.is_open())
        simpleErrorExit("Failed to open file");

    file.seekg(0, file.end);
    int length = file.tellg();

    if (length < 0)
        simpleErrorExit("Failed to get length");
    file.seekg(0, file.beg);

    fileBuffer = new char [length];

    if (!file.read(fileBuffer,length))  {
        file.close();
        simpleErrorExit("Failed to read");
    }

    send(user.getSocket(), fileBuffer, length, 0); //we don't user setBackMSG because of possible size

    file.close();
    delete[] fileBuffer;
}

void        Server::QUIT(User &user, std::string content) {

    if (content == "closed" || content == "crushed")
        user.quitServer(Users, content);
    else if (commandsParse.size() == 1)
        user.quitServer(Users, std::string("no reason"));
    else
        user.quitServer(Users, content.substr(commandsParse[0].size() + 1, content.size()));

    user.quitChannels(Channels);
    Users.erase(user.getSocket());

    close(user.getSocket());
    FD_CLR(user.getSocket(), &master);
    FD_CLR(user.getSocket(), &readSockets);
    FD_CLR(user.getSocket(), &writeSockets);
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