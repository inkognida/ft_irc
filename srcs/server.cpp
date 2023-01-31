#include "../headers/irc.hpp"

bool work = true;

void sigHandler(int num) {
    (void)num;
    work = false;
}

int acceptConnection(int serverSocket, std::map<int, std::string> &Users) {
    struct sockaddr_in client;
    socklen_t client_size = sizeof(client);
    int socketClient = accept(serverSocket, (struct sockaddr*)&client, &client_size);

    if (socketClient < 0) {
        std::cout << "Failed to create accept with socketClient" << std::endl;
        exit(EXIT_FAILURE);

        //TODO HANDLE ERROR
    }

    char host[NI_MAXHOST]; // Client's remote name
    char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << host << " connected on port: " << service << std::endl;
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port: " << ntohs(client.sin_port) << std::endl;
    }

    Users[socketClient];
//    Users[socketClient] = "TESTING"; // TEST example

    return socketClient;
}

void executeServer(int serverSocket, std::map<int, std::string> &Users) {
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 5;

    fd_set master;
    fd_set read;
    fd_set write;

    FD_ZERO(&master);
    FD_SET(serverSocket, &master);

    while (work) {
        read = master;
        write = master;

        if (select(FD_SETSIZE, &read, &write, NULL, &timeout) == -1) {
            std::cout << "Failed to select()" << std::endl;
            close(serverSocket);
            // TODO check memory
            exit(EXIT_FAILURE);
        } else {
            for (int i = 0; i < FD_SETSIZE; i++) {
                if (FD_ISSET(i, &read)) {

                    // accept new connection
                    if (i == serverSocket) {
                        int clientSocket = acceptConnection(serverSocket, Users);
                        FD_SET(clientSocket, &master);

                        std::string welcomeMsg = "Welcome to the IRC Server!\n";
                        send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
                    } else {
                        // receive
                        char buf[4096];
                        memset(buf, 0, 4096);

                        int bytes = recv(i, buf, 4096, 0);
                        Users[i] = std::string(buf, 0, bytes);
                        std::cout << std::string(buf, 0, bytes);

                        // TODO handleFunction
                    }
                }
                if (FD_ISSET(i, &write)) {
                    // TODO sendFunction
                    for (int j = 0; j < FD_SETSIZE; j++) {
                        if (!Users[j].empty() && j != i) {
                            send(i, Users[j].data(), Users[j].size(), 0);
                            Users.erase(j); // handle only 2 users because of iterations -> delete and after wanna to send
                        }
                    }
                } else {
                    continue;
                }
            }
        }
    }
}

void server(int port, std::string password) {
    (void)password;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //IPV4 TCP

    if (serverSocket == -1) {
        std::cout << "Failed to create a socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addrServer;

    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&addrServer, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind the socket" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, SOMAXCONN) < 0) {
        std::cout << "Failed to listen the socket" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::map<int, std::string> Users; // TODO class USER
//    std::map<std::string, std::string> Channels; // TODO class CHANNEL

    executeServer(serverSocket, Users);
}