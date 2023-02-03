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

    return 0;
}