#include "../headers/irc.hpp"

void    Server::backMSG(User &user, int code, std::string cmd) {
    std::string tmp = SERVER + std::to_string(code) + " " + cmd;
    switch (code) {
        case ERR_NEEDMOREPARAMS:
            user.setBackMSG(tmp + " wrong parameters");
            return ;
        case ERR_ALREADYREGISTERED:
            user.setBackMSG(tmp + " Unauthorized command (already registered)");
            return ;
        case ERR_PASSWDMISMATCH:
            user.setBackMSG(tmp + " Password incorrect");
            return ;
        case ERR_NONICKNAMEGIVEN:
            user.setBackMSG(tmp + " no nickname given, use NICK [arg]");
            return ;
        case ERR_ERRONEUSNICKNAME:
            user.setBackMSG(tmp + " Erroneous nickname");
            return ;
        case ERR_NICKNAMEINUSE:
            user.setBackMSG(tmp + " Nickname is already in use");
            return ;
        case ERR_NOTREGISTERED:
            user.setBackMSG(tmp + " You need to be registered (custom error)");
            return ;
        case ERR_NOORIGIN:
            user.setBackMSG(tmp + " No origin specified");
            return ;
        case ERR_BADCHANMASK:
            user.setBackMSG(tmp + " Bad channel mask");
            return ;
        case ERR_BADCHANNELKEY:
            user.setBackMSG(tmp + " Cannot join channel (+k)");
            return ;
        case RPL_NOTOPIC:
            user.setBackMSG(tmp + " No topic is set");
            return ;
        case ERR_CHANOPRIVSNEEDED:
            user.setBackMSG(tmp + " You're not channel operator");
            return ;
        case ERR_NOTONCHANNEL:
            user.setBackMSG(tmp + " You're not on that channel");
            return ;
        case ERR_NOSUCHNICK:
            user.setBackMSG(tmp + " There was no such nickname");
            return ;
        case ERR_UNKNOWNMODE:
            user.setBackMSG(tmp + " is unknown mode char to me for");
            return ;
        case ERR_USERONCHANNEL:
            user.setBackMSG(tmp + " is already on channel");
            return ;
        case ERR_NOSUCHCHANNEL:
            user.setBackMSG(tmp + " No such channel");
            return ;
        case ERR_INVITEONLYCHAN:
            user.setBackMSG(tmp + " Cannot join channel (+i)");
            return ;
        case ERR_CANNOTSENDTOCHAN:
            user.setBackMSG(tmp + " Cannot send to channel");
            return ;
        case ERR_NORECIPIENT:
            user.setBackMSG(tmp + " No recipient given (<command>)");
            return ;
        case ERR_NOTEXTTOSEND:
            user.setBackMSG(tmp + " No text to send");
            return ;
        case ERR_CHANNELISFULL:
            user.setBackMSG(tmp + " Can't join the channel");
            return ;
        case ERR_USERNOTINCHANNEL:
            user.setBackMSG(tmp + " They aren't on that channel") ;
            return ;
    }
}

void    Server::simpleErrorExit(std::string error) {
    std::cout << error << std::endl;
    exit(EXIT_FAILURE);
}
