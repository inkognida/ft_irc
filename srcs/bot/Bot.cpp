#include "../../headers/irc.hpp"

struct srv {
    int             fd;
    std::string     hostname;
    std::string     pswd;

    int             port;
    std::string     token;
    sockaddr_in     addr;
};

class Bot {
public:
    Bot();
    ~Bot();

    // IRC
    void                            initIrc();
    void                            authIrc();
    bool                            formRequest(std::string);
    void                            formRespond();
    void                            formRespondErr(std::string);


    // API
    std::string                     getApiRespond(std::string, std::string);

    //tools
    void                            hostnameIP(std::string);
    void                            err(std::string);
    void                            sigHand(int);
    std::string                     recvFunc(int);

    // extra
    void                            removeSubstrs(std::string, std::string&);
    void                            parseBody(std::string);

    // getters
    int                             getIrcFd(void) { return this->irc.fd; };
    int                             getApiFd(void) { return this->api.fd; };

    std::string                     getCmd(void) { return this->ircRespond[0]; };
    std::string                     getLat(void) { return this->ircRespond[1]; };
    std::string                     getLon(void) { return this->ircRespond[2]; };

private:
    srv                                 api;
    srv                                 irc;
    char                                ip[4096];

    std::string                         lon;
    std::string                         lat;

    std::string                         apiResponse;
    std::string                         ircResponse;

    std::vector<std::string>            ircRespond;

};

Bot::Bot() {
    irc.hostname = "localhost";
    irc.port = 6667;
    irc.pswd = 123;

    api.hostname = "api.openweathermap.org";
    api.port = 80;
    api.token = "0162fdab085f4b6ab262e76ffc309667";
}

Bot::~Bot() {}

void            Bot::initIrc() {
    irc.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (irc.fd < 0)
        err("Failed to create socket");

    irc.addr.sin_family = AF_INET;
    irc.addr.sin_port = htons(irc.port);

    hostnameIP(irc.hostname);
    irc.addr.sin_addr.s_addr = inet_addr(ip);
    memset(ip, 0, 4096);

    if (connect(irc.fd, (struct sockaddr*)&irc.addr, sizeof(irc.addr)) < 0)
        err("Failed to connect");

    if (fcntl(irc.fd, F_SETFL, O_NONBLOCK) < 0)
        err("Failed to fcntl irc");
}

void            Bot::err(std::string reason) {
    std::cout << reason << std::endl;
    exit(1);
}

void            Bot::hostnameIP(std::string hostname) {
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in  *h;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    const char *host = hostname.c_str();
    int rv = getaddrinfo(host, "http", &hints, &servinfo);

    if (rv != 0)
        err("Failed to getaddrinfo");

    for (p = servinfo; p != NULL; p = p->ai_next) {
        h = (struct sockaddr_in *) p->ai_addr;
        strcpy(ip, inet_ntoa(h->sin_addr));
    }

    freeaddrinfo(servinfo);
}

std::string     Bot::getApiRespond(std::string lat, std::string lon) {
    // CONNECTION
    api.fd = socket(AF_INET, SOCK_STREAM, 0);

    if (api.fd < 0)
        err("Failed to create socket");

    api.addr.sin_family = AF_INET;
    api.addr.sin_port = htons(api.port);

    hostnameIP(api.hostname);
    api.addr.sin_addr.s_addr = inet_addr(ip);
    memset(ip, 0, 4096);

    if (connect(api.fd, (struct sockaddr*)&api.addr, sizeof(api.addr)) < 0)
        err("Failed to connect");

    // REQUEST
    std::stringstream in;
    char buf[2048] = {0};
    int rd;

    in << "GET /data/2.5/weather?lat=" << lat << "&lon=" << lon
       << "&appid=" << "0162fdab085f4b6ab262e76ffc309667" << " HTTP/1.1\r\n"
       << "Host: api.openweathermap.org\r\n" <<
       "Accept: application/json\r\n" << "Connection: close\r\n" << "\r\n";

    if (send(api.fd, in.str().c_str(), in.str().length(), 0) < 0)
        err("Failed to send");

    rd = read(api.fd, buf, 2047);
    if (rd == -1)
        return "";

    std::string tmp(buf);
    std::string apiResponse;

    size_t found = tmp.find("name");
    if (found != std::string::npos) {
        found += std::string("name: ").length();
        while (tmp[found] != ',') {
            apiResponse += tmp[found];
            found++;
        }
        apiResponse += "\n";
    } else
        apiResponse = "Something went wrong\n";

    memset(buf, 0, 2048);
    close(api.fd);
    return apiResponse;
}

std::string     Bot::recvFunc(int fd) {
    char buf[2048];
    memset(buf, 0, 2048);

    int MAX_LEN = 4096;
    unsigned total_bytes_received = 0;

    int rd = recv(fd, buf+total_bytes_received, MAX_LEN, 0);
    if (rd == 0) // socket closed by peer
        err("rd");
    else if (rd == -1) {
        if ((EAGAIN == errno) || (EWOULDBLOCK == errno)) // no data to be read on socket
            usleep(10);
        else
            err("Errno");
    } else
        total_bytes_received += rd;

    write(1, buf, total_bytes_received);
    return std::string(buf);
}

void            Bot::authIrc() {
    std::string userInput;
    for (int i = 0; i < 3; i++) { // TODO use i < 3
        getline(std::cin, userInput);
        if (send(irc.fd, userInput.c_str(), userInput.length() + 1, 0) < 0)
            exit(1);
    }
}

bool work = true;
void            Bot::sigHand(int num) {
    (void)num;
    work = false;
}

void            Bot::removeSubstrs(std::string pattern, std::string& str) {
    std::string::size_type i = str.find(pattern);
    while (i != std::string::npos) {
        str.erase(i, pattern.length());
        i = str.find(pattern, i);
    }
}

void            Bot::parseBody(std::string body) {
    ircRespond.clear();

    removeSubstrs(":PUSSY = ", body);
    removeSubstrs(":", body);

    body.erase( std::remove(body.begin(), body.end(), '\r'), body.end());
    body.erase( std::remove(body.begin(), body.end(), '\n'), body.end());


    char *tmp = std::strtok(const_cast<char*>(body.c_str()), " ");
    while (tmp != nullptr) {
        ircRespond.push_back(std::string(tmp));
        tmp = std::strtok(NULL, " ");
    }
}


bool            Bot::formRequest(std::string body) {
    parseBody(body);

    ircRespond[0] = "PRIVMSG " + ircRespond[0] + " ";
    if (ircRespond.size() != 3)
        return false;

    if (ircRespond[1].size() != 5 || ircRespond[2].size() != 5)
        return false;

    // F**K DRY
    for (size_t i = 0; i < ircRespond[1].size(); i++) {
        if (i == 2 && ircRespond[1][i] != '.')
            return false;
        else if (i != 2 && !isdigit(ircRespond[1][i]))
            return false;
    }

    for (size_t i = 0; i < ircRespond[2].size(); i++) {
        if (i == 2 && ircRespond[2][i] != '.')
            return false;
        else if (i != 2 && !isdigit(ircRespond[2][i]))
            return false;
    }
    return true;
}

void            Bot::formRespond() {
    std::string response = getCmd() + Bot::getApiRespond(getLat(), getLon());
    int sd = send(irc.fd, response.c_str(), response.length()+1, 0);
    if (sd < 0)
        err("Failed to send");
}

void            Bot::formRespondErr(std::string body) {
    parseBody(body);

    ircRespond[0] = "PRIVMSG " + ircRespond[0] + " wrong request, usage: PRIVMSG bot lat lon";

    int sd = send(irc.fd, getCmd().c_str(), getCmd().length()+1, 0);
    if (sd < 0)
        err("Failed to send");
}

int main() {
    Bot bot;

    bot.initIrc();
    bot.authIrc();

    usleep(10);
    while (work) {
        std::string tmp = bot.recvFunc(bot.getIrcFd());
        if (tmp.length() != 0 && tmp.find(":PUSSY") != std::string::npos) {
            if (!bot.formRequest(tmp)) {
                bot.formRespondErr(tmp);
                continue;
            } // 0 - PRIVMSG, 1 - lat, 2 - lon
            bot.formRespond();
        }
    }
    return 0;
}

