#ifndef USER_HPP
#define USER_HPP

#include "irc.hpp"

class User {
public:

    User();

    ~User();

    int getSocket() const;

    std::string getNickname() const;

    void setSocket(int);

    void setHostname(std::string);
    std::string getHostname(void);

    void setMessage(std::string);
    std::string getMessage(void);

    void                        setCommandsToParse(std::string);
    std::vector<std::string>    getCommandsParsed();

private:
    int                         socket;
    std::string                 hostname;
    std::string                 nickname;
    std::string                 message;

    std::vector <std::string>   commandsParsed;
    std::string                 commandsToParse;

    std::vector<std::string>    commandsAvaliable;

};

#endif