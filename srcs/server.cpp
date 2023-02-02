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

    FD_ZERO(&master);
    FD_SET(serverSocket, &master);
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

void    Server::handleConnection(int userSocket) {
    char buffer[4096];

    memset(buffer, 0, 4096);
    int readBytes = recv(userSocket, buffer, 4096, 0);

    if (readBytes == -1)
        quitConnection("crushed", userSocket);
    if (readBytes == 0)
        quitConnection("closed", userSocket);
    else {
        Users[userSocket].setCommandsToParse(std::string(buffer, 0, readBytes));
//        FD_CLR(userSocket, &master);
    }
    memset(buffer, 0, 4096);
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
}