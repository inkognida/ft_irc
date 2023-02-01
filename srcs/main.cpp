# include "../headers/irc.hpp"

struct port_password {
    int         port;
    std::string password;
};


port_password parser(int argc, char **argv) {
    port_password input;
    int port;

    port = std::atoi(argv[argc - 2]);

    if (port >= 1 && port <= 65535 && argv[argc - 1]) {
        input.port = port;
        input.password = argv[argc - 1];
    } else {
        std::cout << "Wrong port or empty password" << std::endl;
        exit(1);
    }

    return input;
}

int main(int argc, char **argv) {

    if (argc != 3) {
        std::cout << "Wrong input" << std::endl;
        exit(EXIT_FAILURE);
    }

    port_password input = parser(argc, argv);

    Server server(input.port, input.password);

    server.execute();

    // SIMPLE SENDING CHECK
/*  char buf[4096];
    struct sockaddr_in client;
    socklen_t client_size = sizeof(client);
    int socketClient = accept(serverSocket, (struct sockaddr*)&client, &client_size);

    if (socketClient < 0) {
        std::cout << "Failed to create accept with socketClient" << std::endl;
        exit(EXIT_FAILURE);

        //TODO HANDLE ERROR
    }
    while (work) {
        memset(buf, 0, 4096);

        int bytes = recv(socketClient, buf, 4096, 0);

        std::cout << std::string(buf, 0, bytes) << std::endl;

        send(socketClient, buf, bytes + 1, 0);
    }
*/


/*    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //IPV4 TCP

    if (serverSocket == -1) {
        std::cout << "Failed to create a socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addrServer;

    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(input.port);

    if (bind(serverSocket, (struct sockaddr*)&addrServer, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind the socket" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, SOMAXCONN) < 0) {
        std::cout << "Failed to listen the socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 5;

    fd_set master;
    fd_set read;
    fd_set write;

    FD_ZERO(&master);
    FD_SET(serverSocket, &master);

    while (work) {

        // make read/write sockets to all current sockets
        read = master;
        write = master;

        if (select(FD_SETSIZE, &read, &write, NULL, &timeout) == -1) {
            std::cout << "Failed to select()" << std::endl;
            exit(EXIT_FAILURE);
            //TODO handle memory errors and other staff
        } else {
            for (int i = 0; i < FD_SETSIZE; i++) {
                if (FD_ISSET(i, &read)) {

                    // accept new connection
                    if (i == serverSocket) {
                        int clientSocket = acceptConnection(serverSocket);
                        FD_SET(clientSocket, &master);

                        std::string welcomeMsg = "Welcome to the Awesome Chat Server!\n";
                        send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
                    } else {

                        // receive
                        char buf[4096];
                        memset(buf, 0, 4096);

                        int bytes = recv(i, buf, 4096, 0);
                        std::cout << std::string(buf, 0, bytes) << std::endl;

                    }
                }

                if (FD_ISSET(i, &write)) {
                    // send
                    continue;
                }

                else {
                    // NO SUCH SOCKET IN READ/WRITE SOCKETS POLL
                    continue;
                }
            }
        }
    }
*/

//    std::cout << "HERE" << std::endl;
//    FD_CLR(serverSocket, &master);
//    close(serverSocket);
//
//    FD_ZERO(&master);
//    FD_ZERO(&read);
//    FD_ZERO(&write);

    return 0;
}