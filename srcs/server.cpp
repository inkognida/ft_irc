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

    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(sockaddr)) < 0) {
        close(serverSocket);
        simpleErrorExit("Failed to bind the socket");
    }

    if (listen(serverSocket, SOMAXCONN) < 0) {
        close(serverSocket);
        simpleErrorExit("Failed to listen the socket");
    }

//    server hostname -> output on server
/*    char host[NI_MAXHOST];
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
    else {
        std::cerr << "can't get server information ?? \n";
        exit(EXIT_FAILURE);
    }
*/

    FD_ZERO(&readSockets);
    FD_ZERO(&writeSockets);
    FD_ZERO(&master);

    FD_SET(serverSocket, &master);

    commands["PASS"] = 1;
    commands["NICK"] = 2;
    commands["USER"] = 3;
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

    std::string welcomeMsg = "Welcome to the IRC Server!\n";
    send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

    FD_SET(clientSocket, &master);
}

void    Server::quitConnection(std::string reason, int userSocket) {
    std::cout << "Client quit: " << reason << std::endl;
    std::string msg = ":" + Users[userSocket].getNickname(); // TODO + "!" + Users[userSocket].getUsername()
    // + "@" + "Users[userSocket].getHostname()
    Users.erase(userSocket);
    std::map<int, User>::iterator begin = Users.begin();
    std::map<int, User>::iterator end = Users.end();

    while (begin != end) {
        begin->second.setMessage(msg);
        begin++;
    }

    // TODO quit channels
    close(userSocket);
    FD_CLR(userSocket, &master);
}

void    Server::backMSG(User &user, int code, std::string cmd) {
    switch (code) {
        case 001:
            user.setBackMSG(std::string("Welcome to the ") + SERVER + std::to_string(code) +
            std::string(" Network") + user.getNickname() +
            "!" +user.getUser() + "@" + user.getHostname()); // TODO data of creation server, code 004, 005, fix CODE
            return ;
        case 461:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " wrong parameters, use PASS [arg]");
            return ;
        case 462:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " Unauthorized command (already registered)");
            return ;
        case 464:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " Password incorrect");
            return ;
        case 431:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " no nickname given, use NICK [arg]");
            return ;
        case 432:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " Erroneous nickname");
            return ;
        case 433:
            user.setBackMSG(SERVER + std::to_string(code) + " " + cmd + " Nickname is already in use");
            return ;
    }
}

void    Server::PASS(User &user) {
    if (commandsParse.size() != 2)
        backMSG(user, 461, user.getCmd());
    else if (!user.getPassword().empty() && !user.getNickname().empty() &&
        !user.getUser().empty()) {
        backMSG(user, 462, user.getCmd());
    }
    else if (commandsParse[1] != password)
        backMSG(user, 464, user.getCmd());
    else
        user.setPassword(password);
}

int    Server::correctNICK(std::string nick) {
    if (nick.size() > 20) {
        return 1;
    }

    for (size_t i = 0; i < nick.size(); i++) {
        if (!isalpha(nick[i]) && !isdigit(nick[i]) && nick[i] != '_')
            return 1;
    }

    // TODO doesnt work with multiple nicknames sending from one socket -> need to fix
    std::map<int, User>::iterator begin = Users.begin();
    std::map<int, User>::iterator end = Users.end();

    while (begin != end) {
        if (begin->second.getNickname() == nick)
            return 2;
        begin++;
    }

    return 3;
}

void    Server::NICK(User &user) {
    if (commandsParse.size() != 2) {
        backMSG(user, 431, user.getCmd());
        return ;
    }

    switch (correctNICK(commandsParse[1])) {
        case 1:
            backMSG(user, 432, user.getCmd());
            return ;
        case 2:
            backMSG(user, 433, user.getCmd());
            return ;
        case 3:
            user.setNickname(commandsParse[1]);
            return ;
    }
    // TODO blocked nicknames
}

void Server::parseCommands(std::string content) {
    User &user = Users.find(clientSocket)->second;
    content.erase( std::remove(content.begin(), content.end(), '\r'), content.end());
    content.erase( std::remove(content.begin(), content.end(), '\n'), content.end());

    if (!content.empty()) {
        char *tmp = std::strtok(const_cast<char*>(content.c_str()), " ");

        while (tmp != nullptr) {
            commandsParse.push_back(std::string(tmp));
            tmp = std::strtok(NULL, " ");
        }

        int cmd = commands[commandsParse[0]];
        user.setCmd(commandsParse[0]);

        if (!cmd)
            exit(EXIT_FAILURE); // TODO unknown command send some message

        if (cmd > 3 && (user.getPassword().empty() || user.getNickname().empty()
                        || user.getUser().empty())) {
            std::cout << "No registration" << std::endl;
        } // TODO return message about no registration process
        switch (cmd) {
            case 1:
                PASS(user);
                break   ;
            case 2:
                NICK(user);
                std::cout << user.getBackMSG() << std::endl; // showing current output from backMSG
                break ;
        }

//        for (size_t i = 0; i < commandsParse.size(); i++) {
//            std::cout << commandsParse[i];
//        }

        commandsParse.clear();
    }
}

void    Server::handleConnection(int userSocket) {
    char buffer[4096];

    memset(buffer, 0, 4096);
    int rd = recv(userSocket, buffer, 4096, 0);

    if (rd == -1)
        quitConnection("crushed", userSocket);
    if (rd == 0)
        quitConnection("closed", userSocket);
    else {
        parseCommands(std::string(buffer).c_str());
        memset(buffer, 0, 4096);
    }
}

void    Server::simpleErrorExit(std::string error) {
    std::cout << error << std::endl;
    exit(EXIT_FAILURE);
}


void    Server::sendConnection(int userSocket) {
    std::cout << "msg sent: " << Users[userSocket].getMessage() << std::endl;
    send(userSocket, Users[userSocket].getMessage().c_str(),
         Users[userSocket].getMessage().size(), 0);
    Users[userSocket].setMessage("");
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
            if (FD_ISSET(i, &writeSockets) && !Users[i].getMessage().empty())
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