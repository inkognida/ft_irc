#ifndef SERVER_HPP
#define SERVER_HPP

class User;

# include "irc.hpp"

#define SERVER ":PUSSY "
#define RPL_WELCOME 001
#define ERR_NOORIGIN 409
// LIST
#define RPL_LIST 322
#define RPL_LISTEND 323
#define RPL_INVITING 341
// NICKNAME
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
// USERNAME
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
// OPER
#define ERR_PASSWDMISMATCH 464
#define ERR_NOOPERHOST 491
// JOIN
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_CHANNELISFULL 471
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_USERONCHANNEL 443
// PART
#define ERR_NOTONCHANNEL 442
// TOPIC
#define RPL_TOPIC 332
#define RPL_TOPICWHOTIME 333
// KICK
#define ERR_USERNOTINCHANNEL 441
#define ERR_BADCHANMASK 476
#define ERR_CHANOPRIVSNEEDED 482
//PRIVMSG
#define ERR_NOSUCHNICK 401
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_NOTREGISTERED 451
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
//MODE
#define ERR_UMODEUNKNOWNFLAG 501

class Server {

public:
    Server(int, std::string);
    ~Server();

    void    execute();
    void    sigHandler(int);
    void    acceptConnection();
    void    handleConnection(int userSocket);
    void    sendConnection(int userSocket);

    //extra
    void    simpleErrorExit(std::string);

    // getters
    int     getSocket(void) { return this->serverSocket; }

    // parse
    void                        parseCommands(std::string);

    // execution
    void                        PASS(User&);
    void                        NICK(User&);
    void                        USER(User&);
    void                        PRIVMSG(User&, std::string);
    void                        PING(User&, std::string);
    void                        NOTICE(User&, std::string);
    void                        QUIT(User&, std::string);

    //tools for execution
    int                         correctNICK(std::string);
    int                         correctUSER(std::string, int, User&);
    void                        backMSG(User&, int, std::string);
private:
    std::string                         password;
    int                                 port;

    int                                 serverSocket;
    sockaddr_in                         serverAddress;

    fd_set                              master;
    fd_set                              readSockets;
    fd_set                              writeSockets;

    timeval                             timeout;

    std::map<int, User>                 Users;

    std::vector <std::string>           commandsParse;
    std::map <std::string, int>         commands;

    int                                 clientSocket;
    sockaddr_in                         clientAddress;
};

#endif