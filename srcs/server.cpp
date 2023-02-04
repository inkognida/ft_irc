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
                          + std::string(" QUIT ") + content + "\n";
    } else
        msg = ":" + user.getNickname() + "!" + user.getRealname() + "@" + user.getHostname()
                          + std::string(" QUIT ") + content.substr(commandsParse[0].size() + 1, content.size()) + "\n";


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
    switch (code) {
        case RPL_WELCOME:
            user.setBackMSG(std::string("Welcome to the ") + SERVER + std::to_string(code) +
            std::string(" Network") + user.getNickname() +
            "!" +user.getUser() + "@" + user.getHostname()); // TODO data of creation server, code 004, 005, fix CODE
            return ;
        case ERR_NEEDMOREPARAMS:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " wrong parameters");
            return ;
        case ERR_ALREADYREGISTERED:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " Unauthorized command (already registered)");
            return ;
        case ERR_PASSWDMISMATCH:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " Password incorrect");
            return ;
        case ERR_NONICKNAMEGIVEN:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " no nickname given, use NICK [arg]");
            return ;
        case ERR_ERRONEUSNICKNAME:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " Erroneous nickname");
            return ;
        case ERR_NICKNAMEINUSE:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " Nickname is already in use");
            return ;
        case ERR_NOTREGISTERED:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " You need to be registered (custom error)");
        case ERR_NOORIGIN:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " No origin specified");
    }
}

void    Server::PING(User &user, std::string content) {
    if (commandsParse.size() < 2) {
        backMSG(user, ERR_NOORIGIN, user.getCmd());
        return ;
    }
    user.setBackMSG(SERVER + std::string("PONG ") + content.substr(commandsParse[0].size() + 1, content.size()) + "\n");
}

void    Server::PRIVMSG(User &user, std::string content) {
    if (commandsParse[1].empty()) {
        backMSG(user, ERR_NORECIPIENT, user.getCmd());
        return ;
    }
    if (commandsParse[2].empty()) {
        backMSG(user, ERR_NOTEXTTOSEND, user.getCmd());
        return ;
    }

    std::map<int, User>::iterator begin = Users.begin();
    std::map<int, User>::iterator end = Users.end();

    while (begin != end) {
        if (begin->second.getNickname() == commandsParse[1]) {
            begin->second.setBackMSG(content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size()) + "\n");
            user.setBackMSG(content.substr(commandsParse[0].size() + commandsParse[1].size() + 2, content.size()) + "\n");
            return ;
        }
        begin++;
    }

    backMSG(user, ERR_NORECIPIENT, user.getCmd());
    return ;
}

void    Server::NOTICE(User &user, std::string content) {
    (void)user;
    (void)content;
    return ; //TODO implement this func
}

void Server::parseCommands(std::string content) {
    User &user = Users.find(clientSocket)->second;
    content.erase( std::remove(content.begin(), content.end(), '\r'), content.end());
    content.erase( std::remove(content.begin(), content.end(), '\n'), content.end());

    if (!content.empty()) {
        std::string content_ = content; // PRIVMSG text argument
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
            std::cout << user.getBackMSG() << std::endl;
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
                PRIVMSG(user, content_);
                break ;
            case 5:
                PING(user, content_);
                break ;
            case 6:
                NOTICE(user, content_);
                break ;
            case 7:
                QUIT(user, content_);
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
        parseCommands(std::string(buffer).c_str());
        commandsParse.clear();
        memset(buffer, 0, 4096);
    }
}

void    Server::simpleErrorExit(std::string error) {
    std::cout << error << std::endl;
    exit(EXIT_FAILURE);
}


void    Server::sendConnection(int userSocket) {
    std::cout << "msg sent: " << Users[userSocket].getBackMSG();
    send(userSocket, Users[userSocket].getBackMSG().c_str(),
         Users[userSocket].getBackMSG().size(), 0);
    Users[userSocket].clearBackMSG();
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

    for (size_t i = 0; i < Users.size(); i++) {
        close(Users[i].getSocket());
    }

    close(serverSocket);

    FD_ZERO(&master);
    FD_ZERO(&readSockets);
    FD_ZERO(&writeSockets);
}