#ifndef SERVER_HPP
#define SERVER_HPP

class User;

# include "irc.hpp"

class Server {

public:
    Server(int, std::string);
    ~Server();

    void    execute();
    void    sigHandler(int);
    void    acceptConnection();
    void    handleConnection(int userSocket);
    void    sendConnection(int userSocket);
    void    quitConnection(std::string, int);

    //extra
    void    simpleErrorExit(std::string);



private:
    std::string password;
    int         port;

    int         serverSocket;
    sockaddr_in serverAddress;

    fd_set      master;
    fd_set      readSockets;
    fd_set      writeSockets;

    timeval     timeout;

    std::map<int, User> Users;

    int         clientSocket;
    sockaddr_in clientAddress;
};

#endif