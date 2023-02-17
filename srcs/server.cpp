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

    // HANDLED COMMANDS
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
    commands["LIST"] = 13;
    commands["NAMES"] = 14;
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
    Users[clientSocket].setNewUser(true);

    send(clientSocket, std::string(WELCOME_MESSAGE).c_str(), std::string(WELCOME_MESSAGE).size() + 1, 0);
    FD_SET(clientSocket, &master);
}

void Server::parseCommands(std::string content, int userSocket) {
    User &user = Users.find(userSocket)->second;
    content.erase( std::remove(content.begin(), content.end(), '\r'), content.end());
    content.erase( std::remove(content.begin(), content.end(), '\n'), content.end());

    if (!content.empty()) {
        std::string args = content; // text argument
        char *tmp = std::strtok(const_cast<char*>(content.c_str()), " ");

        while (tmp != nullptr) {
            commandsParse.push_back(std::string(tmp));
            tmp = std::strtok(NULL, " ");
        }

        int cmd = commands[commandsParse[0]];

        if (!cmd)
            return ;

        user.setCmd(commandsParse[0]);

        if (cmd > 3 && user.getNewUser()) { // TODO turn on auth process -> cmd > 3
            backMSG(user, ERR_NOTREGISTERED, user.getCmd());
            return ;
        }

        switch (cmd) {
            case 1:
                PASS(user);// test passed
                break   ;
            case 2:
                NICK(user);// test passed
                break ;
            case 3:
                USER(user, args);// test passed
                break ;
            case 4:
                PRIVMSG(user, args);// test passed
                break ;
            case 5:
                PING(user, args);// test passed
                break ;
            case 6:
                NOTICE(user,args);// test passed
                break ;
            case 7:
                QUIT(user, args);// test passed
                break ;
            case 8:
                JOIN(user);// test passed
                break ;
            case 9:
                TOPIC(user, args);// test passed
                break ;
            case 10:
                MODE(user);// test passed
                break ;
            case 11:
                KICK(user, args);// test passed
                break ;
            case 12:
                INVITE(user);// test passed
                break ;
            case 13:
                LIST(user); // test passed
                break ;
            case 14:
                NAMES(user); // test passed
                break ;
        }

        if (user.getNewUser() && user.getRegistration()) {
            fileTransfer(user);
            user.setNewUser(false);
            return ;
        }
    }
}

void    Server::handleConnection(int userSocket) {
    char buffer[4096];

    memset(buffer, 0, 4096);
    int rd = recv(userSocket, buffer, 4096, 0);

    // TODO enable file sending from client if needed (use key word file)
    /*
    if (std::string(buffer, 4) == "file")
        std::cout << std::string(buffer) << std::endl;
    */

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

void    Server::sendConnection(int userSocket) {
    response = Users[userSocket].getResponseMSG();
    std::cout << "msg sent: \n" << response;
    if (send(userSocket, response.c_str(), response.size(), 0) < 0)
        simpleErrorExit("Failed to send");
    Users[userSocket].clearBackMSG();
    Users[userSocket].clearResponse();
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

    std::map<int, User>::const_iterator begin = Users.begin();
    std::map<int, User>::const_iterator end = Users.end();

    while (begin != end) {
        close(begin->second.getSocket());
        begin++;
    }
    close(serverSocket);

    FD_ZERO(&master);
    FD_ZERO(&readSockets);
    FD_ZERO(&writeSockets);
}