#include "../headers/irc.hpp"

bool work = true;

void Server::sigHandler(int num) {
    (void)num;
    work = false;
}

Server::Server(int port_, std::string password_) {
    this->port = port_;
    this->password = password_;

    timeout.tv_sec = 0;
    timeout.tv_usec = 5;
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); //IPV4 TCP

    if (serverSocket == -1)
        simpleErrorExit("Failed to create a socket");

    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    int on = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(int)) < 0) {
        close(serverSocket);
        simpleErrorExit("Failed to setsockopt");
    }

    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(sockaddr)) < 0) {
        close(serverSocket);
        simpleErrorExit("Failed to bind the socket");
    }

    if (listen(serverSocket, SOMAXCONN) < 0) {
        close(serverSocket);
        simpleErrorExit("Failed to listen the socket");
    }

    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0) {
        close(serverSocket);
        simpleErrorExit("Failed to fcntl");
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
    if (getnameinfo((sockaddr*)&serverAddress, sizeof(serverAddress), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        if (!std::strcmp(service, "ircd"))
            std::cout << host << " SERVER PORT : " << "6667\n";
        else
            std::cout << host << " SERVER PORT : " << service << std::endl;
    }
    else
        simpleErrorExit("Can't get server info");

    FD_ZERO(&readSockets);
    FD_ZERO(&writeSockets);
    FD_ZERO(&master);

    FD_SET(serverSocket, &master);

    commands["PASS"] = 1;
    commands["NICK"] = 2;
    commands["USER"] = 3;
    commands["PRIVMSG"] = 4;
    commands["PING"] = 5;
    commands["NOTICE"] = 6;
    commands["QUIT"] = 7;
    commands["JOIN"] = 8;
    commands["TOPIC"] = 9;
    commands["MODE"] = 10;
    commands["KICK"] = 11;
    commands["INVITE"] = 12;
}

Server::~Server() {

}

void Server::acceptConnection() {
    socklen_t client_size = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &client_size);

    if (clientSocket < 0)
        simpleErrorExit("Failed to create accept with socketClient");

    char host[NI_MAXHOST]; // Client's remote name
    char service[NI_MAXSERV]; // Service (i.e. port) the client is connected on

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&clientAddress, sizeof(clientAddress), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << host << " connected on port: " << service << std::endl;
    }
    else {
        inet_ntop(AF_INET, &clientAddress.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port: " << ntohs(clientAddress.sin_port) << std::endl;
    }

    Users[clientSocket];
    Users[clientSocket].setSocket(clientSocket);
    Users[clientSocket].setHostname(host);

    // TODO delete/change backMSG
    std::string welcomeMsg = "Welcome to the IRC Server!\n";
    send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

    FD_SET(clientSocket, &master);
}

void    Server::QUIT(User &user, std::string content) {
    std::cout << "User " << user.getNickname() << " left, reason: " << content << std::endl;
    std::string msg;
    if (content == "closed" || content == "crushed") {
       msg = ":" + user.getNickname() + "!" + user.getRealname() + "@" + user.getHostname()
                          + std::string(" QUIT ") + content;
    } else {
        content = content.substr(commandsParse[0].size() + 1, content.size());
        msg = ":" + user.getNickname() + "!" + user.getRealname() + "@" + user.getHostname()
              + std::string(" QUIT ") + content;
    }


    Users.erase(user.getSocket());

    std::map<int, User>::iterator begin = Users.begin();
    std::map<int, User>::iterator end = Users.end();
    while (begin != end) {
        begin->second.setBackMSG(msg);
        begin++;
    }

    // TODO quit channels
    close(user.getSocket());
    FD_CLR(user.getSocket(), &master);
}

void    Server::backMSG(User &user, int code, std::string cmd) {
    std::string tmp = SERVER + std::to_string(code) + " " + cmd;
    switch (code) {
        case RPL_WELCOME:
            user.setBackMSG(std::string("Welcome to the ") + SERVER + std::to_string(code) +
            std::string(" Network") + user.getNickname() +
            "!" +user.getUser() + "@" + user.getHostname()); // TODO data of creation server, code 004, 005, fix CODE
            return ;
        case ERR_NEEDMOREPARAMS:
            user.setBackMSG(tmp + " wrong parameters");
            return ;
        case ERR_ALREADYREGISTERED:
            user.setBackMSG(tmp + " Unauthorized command (already registered)");
            return ;
        case ERR_PASSWDMISMATCH:
            user.setBackMSG(tmp + " Password incorrect");
            return ;
        case ERR_NONICKNAMEGIVEN:
            user.setBackMSG(tmp + " no nickname given, use NICK [arg]");
            return ;
        case ERR_ERRONEUSNICKNAME:
            user.setBackMSG(tmp + " Erroneous nickname");
            return ;
        case ERR_NICKNAMEINUSE:
            user.setBackMSG(tmp + " Nickname is already in use");
            return ;
        case ERR_NOTREGISTERED:
            user.setBackMSG(tmp + " You need to be registered (custom error)");
            return ;
        case ERR_NOORIGIN:
            user.setBackMSG(tmp + " No origin specified");
            return ;
        case ERR_BADCHANMASK:
            user.setBackMSG(tmp + " Bad channel mask");
            return ;
        case ERR_BADCHANNELKEY:
            user.setBackMSG(tmp + " Cannot join channel (+k)");
            return ;
        case RPL_NOTOPIC:
            user.setBackMSG(tmp + " No topic is set");
            return ;
        case ERR_CHANOPRIVSNEEDED:
            user.setBackMSG(tmp + " You're not channel operator");
            return ;
        case ERR_NOTONCHANNEL:
            user.setBackMSG(tmp + " You're not on that channel");
            return ;
        case ERR_NOSUCHNICK:
            user.setBackMSG(tmp + " There was no such nickname in channel");
            return ;
        case ERR_UNKNOWNMODE:
            user.setBackMSG(tmp + " is unknown mode char to me for");
            return ;
        case ERR_USERONCHANNEL:
            user.setBackMSG(tmp + " is already on channel");
            return ;
        case ERR_NOSUCHCHANNEL:
            user.setBackMSG(tmp + " No such channel");
            return ;
        case ERR_INVITEONLYCHAN:
            user.setBackMSG(tmp + " Cannot join channel (+i)");
            return ;

    }
}

void    Server::PING(User &user, std::string content) {
    if (commandsParse.size() < 2) {
        backMSG(user, ERR_NOORIGIN, user.getCmd());
        return ;
    }
    content = content.substr(commandsParse[0].size() + 1, content.size());
    user.setBackMSG(SERVER + std::string("PONG ") + content);
}

void    Server::NOTICE(User &user, std::string content) {
    (void)user;
    (void)content;
    return ; //TODO implement this func
}

void Server::parseCommands(std::string content, int userSocket) {
    User &user = Users.find(userSocket)->second;
    content.erase( std::remove(content.begin(), content.end(), '\r'), content.end());
    content.erase( std::remove(content.begin(), content.end(), '\n'), content.end());

    if (!content.empty()) {
        std::string args = content; // PRIVMSG text argument
        char *tmp = std::strtok(const_cast<char*>(content.c_str()), " ");

        while (tmp != nullptr) {
            commandsParse.push_back(std::string(tmp));
            tmp = std::strtok(NULL, " ");
        }

        int cmd = commands[commandsParse[0]];
        user.setCmd(commandsParse[0]);

        if (!cmd)
            return ;

        if (cmd > 666 && (user.getPassword().empty() || user.getNickname().empty() // TODO turn on the auth process (666) / add REGISTRATION flag for RPL_WELCOME
                        || user.getUser().empty())) {
            backMSG(user, ERR_NOTREGISTERED, user.getCmd());
            std::cout << user.getResponseMSG() << std::endl;
            user.clearBackMSG();
            return ;
        }

        switch (cmd) {
            case 1:
                PASS(user);
                break   ;
            case 2:
                NICK(user);
                break ;
            case 3:
                USER(user);
                break ;
            case 4:
                PRIVMSG(user, args);
                break ;
            case 5:
                PING(user, args);
                break ;
            case 6:
                NOTICE(user,args);
                break ;
            case 7:
                QUIT(user, args);
                break ;
            case 8:
                JOIN(user);
                break ;
            case 9:
                TOPIC(user, args);
                break ;
            case 10:
                MODE(user);
                break ;
            case 11:
                KICK(user, args);
                break ;
            case 12:
                INVITE(user);
                break ;
        }
    }
}

void    Server::handleConnection(int userSocket) {
    char buffer[4096];

    memset(buffer, 0, 4096);
    int rd = recv(userSocket, buffer, 4096, 0);

    if (rd == -1)
        QUIT(Users[userSocket], "crushed");
    if (rd == 0)
        QUIT(Users[userSocket], "closed");
    else {
        parseCommands(std::string(buffer).c_str(), userSocket);
        commandsParse.clear();
        memset(buffer, 0, 4096);
    }
}

void    Server::simpleErrorExit(std::string error) {
    std::cout << error << std::endl;
    exit(EXIT_FAILURE);
}


void    Server::sendConnection(int userSocket) {
    response = Users[userSocket].getResponseMSG();
    std::cout << "msg sent: \n" << response;
    send(userSocket, response.c_str(), response.size(), 0);
    Users[userSocket].clearBackMSG();
    Users[userSocket].clearMsg();
}

void Server::execute() {
    while (work) {
        readSockets = master;
        writeSockets = master;

        if (select(FD_SETSIZE, &readSockets, &writeSockets, NULL, &timeout) == -1) {
            close(serverSocket);
            simpleErrorExit("Failed to select()");
        }
        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &readSockets)) {
                if (i == serverSocket)
                    acceptConnection();
                else
                    handleConnection(i);
            }
            if (FD_ISSET(i, &writeSockets) && !Users[i].getBackMSG().empty())
                sendConnection(i);
        }
    }

    for (size_t i = 0; i < Users.size(); i++)
        close(Users[i].getSocket());
    close(serverSocket);

    FD_ZERO(&master);
    FD_ZERO(&readSockets);
    FD_ZERO(&writeSockets);
}