# include <sstream>
# include <string>
# include <cstring>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <map>
# include <set>
# include <csignal>
# include <vector>
# include <algorithm>
# include <sys/time.h>
# include <sstream>
# include <iostream>



bool work = true;

struct port_password {
    int         port;
    std::string password;
};

void sigHandler(int num) {
    (void)num;
    work = false;
}

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
        return 1;
    }

    port_password input = parser(argc, argv);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //IPV4 TCP

    if (sockfd == -1) {
        std::cout << "Failed to create a socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addrServer;

    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(input.port);

    if (bind(sockfd, (struct sockaddr*)&addrServer, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind the socket" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, SOMAXCONN) < 0) {
        std::cout << "Failed to listen the socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (work) {
        int socketClient;
        struct sockaddr_in addrClient;
        socklen_t  csize = sizeof(addrClient);

        socketClient = accept(sockfd, (struct sockaddr *)&addrClient, &csize);
        if (socketClient < 0) {
            exit(EXIT_FAILURE);
        }

        char buffer[100];

    }

    return 0;
}