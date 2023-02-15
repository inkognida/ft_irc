#include "../../headers/irc.hpp"

// TODO analyze
//#include "../../../Users/hardella/.brew/Cellar/nlohmann_json/3.10.0/include/nlohmann/json.hpp"
//using json = nlohmann::json;


//
//class Socket {
//
//public:
//    Socket(std::string hostname, int port, std::string token_ = "") {
//        fd = socket(AF_INET, SOCK_STREAM, 0);
//        if (fd < 0)
//            err("Failed to create socket");
//
//        memset(&addr, 0, sizeof(addr));
//        addr.sin_family = AF_INET;
//        addr.sin_port = htons(port);
//        hostname_to_ip(hostname);
//        addr.sin_addr.s_addr = inet_addr(ip);
//    };
//
//    ~Socket(void) {
//        close(fd);
//    };
//
//    void             hostname_to_ip(std::string hostname) {
//        struct addrinfo hints, *servinfo, *p;
//        struct sockaddr_in  *h;
//
//        memset(&hints, 0, sizeof(hints));
//        hints.ai_family = AF_UNSPEC;
//        hints.ai_socktype = SOCK_STREAM;
//        hints.ai_protocol = IPPROTO_TCP;
//
//        const char *host = hostname.c_str();
//        int rv = getaddrinfo(host, "http", &hints, &servinfo);
//
//        if (rv != 0)
//            err("Failed to getaddrinfo");
//
//        for (p = servinfo; p != NULL; p = p->ai_next) {
//            h = (struct sockaddr_in *) p->ai_addr;
//            if (ping(std::string(inet_ntoa(h->sin_addr))))
//                strcpy(ip, inet_ntoa(h->sin_addr));
//        }
//
//        freeaddrinfo(servinfo);
//    }
//
//    void            toConnect(void) {
//        if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
//            err("Failed to connect");
//    }
//
//    void             toSend(std::string request) {
//        if (send(fd, request.c_str(), request.length(), 0) < 0)
//            err("Failed to send");
//    }
//
//    std::string     toRecv(void) {
//        // TODO accept JSON from API
//        char buffer[4096];
//        memset(buffer, 0, 4096);
//
//        int rd = read(fd, buffer, 4096);
//
//        if (rd < 0)
//            err("Failed to recv");
//
//        std::string tmp(buffer, sizeof (buffer));
//        memset(buffer, 0, 4096);
//        return tmp;
//    };
//
//    int             getFd(void) { return this->fd; };
//
//    // tools
//    void            err(std::string reason) {
//        std::cout << reason << std::endl;
//        exit(1);
//    }
//
//    int             ping(std::string hostIP) {
//        std::string tmp = "ping -c1 -s1 "+hostIP+"  > /dev/null 2>&1";
//        int x = std::system(tmp.c_str());
//
//        if (x == 0)
//            return (1);
//        else
//            return (0);
//    }
//
//private:
//    int             fd;
//
//    char            ip[4096];
//    int             port;
//    std::string     token;
//
//    sockaddr_in     addr;
//};
//
//
//
//
//
//
//
//
//class Bot {
//
//public:
//    Bot();
//    ~Bot();
//
//
//    void                    createConnection();
//    void                    sigHandler(int);
//
//    std::string              requestAPI(std::string lat, std::string lon) {
//        std::stringstream req;
//
//        req << "GET /data/2.5/weather?lat=" << lat << "&lon=" << lon << "&appid=" << tokenAPI << " HTTP/1.1\r\n"
//              "Host: api.openweathermap.org\r\n"
//              "Accept: application/json\r\n"
//              "Connection: close\r\n" << "\r\n";
//
////        socketAPI->toSend(req.str());
//
//        send(socketAPI->getFd(), req.str().c_str(), req.str().length(), 0);
//        char buffer[4096]={0};
//        memset(buffer, 0, 4096);
//
//        int rd = read(socketAPI->getFd(), buffer, 4096);
//
//        if (rd < 0)
//            err("Failed to recv");
//
//        std::string tmp(buffer, sizeof (buffer));
//        std::cout << tmp;
//        memset(buffer, 0, 4096);
//
//
//        json        data = json::parse(req);
//        json::iterator begin = data.begin();
//        json::iterator end = data.end();
//
//        while (begin != end) {
//            std::cout << begin.key();
//            begin++;
//        }
//
//        return responseAPI;
//    }
//
//
//    void            err(std::string reason) {
//        std::cout << "Error: " << reason << std::endl;
//        exit(1);
//    };
//
//    // tools
//
//private:
//    Socket          *socketIRC;
//    Socket          *socketAPI;
//
//    std::string     hostnameIRC;
//    std::string     hostnameAPI;
//
//    int             portIRC;
//    int             portAPI;
//
//    // NO TOKEN for IRC
//    std::string     tokenAPI;
//
//    std::string     responseAPI;
//};
//
//void            Bot::createConnection() {
////    socketIRC = new Socket(hostnameIRC, portIRC);
//    socketAPI = new Socket(hostnameAPI, portAPI, tokenAPI);
//
////    socketIRC->toConnect();
//    socketAPI->toConnect();
//
////    fcntl(socketIRC->getFd(), F_SETFL, O_NONBLOCK);
//    fcntl(socketAPI->getFd(), F_SETFL, O_NONBLOCK);
//}
//
//Bot::Bot() {
//    this->hostnameIRC = "localhost";
//    this->portIRC = 6667;
//
//    this->hostnameAPI = "api.openweathermap.org";
//    this->portAPI = 80;
//    this->tokenAPI = "0162fdab085f4b6ab262e76ffc309667";
//}
//
//Bot::~Bot() {
//    // TODO check memory leaks
//    delete socketAPI;
//    delete socketIRC;
//}
//
//bool work = true;
//
//void Bot::sigHandler(int num) {
//    (void)num;
//    work = false;
//}
//
//
//
//
//int main() {
//    Bot bot;
//
//    bot.createConnection();
//
//    std::string err = bot.requestAPI("64.34", "10.99");
//
//    std::cout << err;
//}

//string Bot::requestAPI( const string &name) {
//    std::stringstream ss;
//
//    ss << "GET " << getLocationURL(name) << " HTTP/1.1\r\n";
//    ss << "\n\n";
//
//    ss << "    GET /data/2.5/weather?lat=44.34&lon=10.99&appid=0162fdab085f4b6ab262e76ffc309667 HTTP/1.1\n"
//          "    Host: api.openweathermap.org" << "\n\n";
//
//    std::cout << ss.str() << std::endl;
//    _APIsocket->tryToSend(ss.str());
//
//    char buf[2048] = {0};
//    int res = read(_APIsocket->getFd(), buf, 2047);
//
//    std::cout << buf << std::endl;
//    if (res != -1) {
//        return string(buf);
//    } else {
//        return "";
//    }
//}

//

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

    void    initApi();
    void    initIrc();

    void    authIrc();
    void    getIrc();

    void    getApi();
    //tools
    void    hostnameIP(std::string);
    void    err(std::string);
    int     ping(std::string);
    void    sigHand(int);

private:
    srv api;
    srv irc;
    char ip[4096];

    std::string lon;
    std::string lat;

    std::string apiResponse;
    std::string ircResponse;

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

void Bot::initApi() {
    api.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (api.fd < 0)
        err("Failed to create socket");
    memset(&api.addr, 0, sizeof(api.addr));
    api.addr.sin_family = AF_INET;
    api.addr.sin_port = htons(api.port);
    hostnameIP(api.hostname);
    api.addr.sin_addr.s_addr = inet_addr(ip);
    memset(ip, 0, 4096);
    if (connect(api.fd, (struct sockaddr*)&api.addr, sizeof(api.addr)) < 0)
        err("Failed to connect api");

    fcntl(api.fd, F_SETFL, O_NONBLOCK);

}

void Bot::initIrc() {
    irc.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (irc.fd < 0)
        err("Failed to create socket");
    memset(&irc.addr, 0, sizeof(irc.addr));
    irc.addr.sin_family = AF_INET;
    irc.addr.sin_port = htons(irc.port);
    hostnameIP(irc.hostname);
    irc.addr.sin_addr.s_addr = inet_addr(ip);
    memset(ip, 0, 4096);
    if (connect(irc.fd, (struct sockaddr*)&irc.addr, sizeof(irc.addr)) < 0)
        err("Failed to connect irc");

    fcntl(irc.fd, F_SETFL, O_NONBLOCK);
}

int             Bot::ping(std::string hostIP) {
    std::string tmp = "ping -c1 -s1 "+hostIP+"  > /dev/null 2>&1";
    int x = std::system(tmp.c_str());
    if (x == 0)
        return (1);
    else
        return (0);
}

void             Bot::err(std::string reason) {
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
        if (ping(std::string(inet_ntoa(h->sin_addr))))
            strcpy(ip, inet_ntoa(h->sin_addr));
    }

    freeaddrinfo(servinfo);
}

void    Bot::getApi() {
    std::stringstream in;

    lat = "64.34";
    lon = "10.99";

    in << "GET /data/2.5/weather?lat=" << lat << "&lon=" << lon << "&appid=" << api.token << " HTTP/1.1\r\n"
              "Host: api.openweathermap.org\r\n"
              "Accept: application/json\r\n"
              "Connection: close\r\n" << "\r\n";

    if (send(api.fd, in.str().c_str(), in.str().length(), 0) < 0)
        err("Failed to send api");

    char buf[4096] = {0};
    int rd = read(api.fd, buf, 4096);

    if (rd < 0)
        err("Failed to read api");

    std::string tmp(buf, sizeof(buf));
    memset(buf, 0, 4096);

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
}

void    Bot::getIrc() {

}

#ifdef __APPLE__
#define IRC_NOSIGNAL SO_NOSIGPIPE
#else
#define IRC_NOSIGNAL MSG_NOSIGNAL
#endif
void    Bot::authIrc() {
    std::string pass = "PASS 123";
    std::string nickname = "NICKNAME bot";
    std::string user = "USER bot bot bot bot";

    if (send(irc.fd, pass.c_str(), pass.length(), 0) < 0)
        err("Failed to auth irc");

    if (send(irc.fd, nickname.c_str(), nickname.length(), 0) < 0)
        err("Failed to auth irc");

    if (send(irc.fd, user.c_str(), user.length(), 0) < 0)
        err("Failed to auth irc");

//    char buf[4096];
//    memset(buf, 0, 4096);
//
//    int rd = recv(irc.fd, buf, 4096, 0);
//    if (rd < 0)
//        err("Failed to recv irc");
//    std::cout << std::string(buf) << std::endl;
}

bool work = true;

void Bot::sigHand(int num) {
    (void)num;
    work = false;
}

int main() {
//    Bot bot;

//    bot.initIrc();

    srv irc;
    irc.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (irc.fd < 0)
        exit(1);
    memset(&irc.addr, 0, sizeof(irc.addr));

    irc.addr.sin_family = AF_INET;
    irc.addr.sin_port = htons(6667);
    irc.addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buf[4096];
    std::string userInput;


    if (connect(irc.fd, (struct sockaddr*)&irc.addr, sizeof(irc.addr)) < 0){
        std::cout << "connect\n";
        exit(1);
    }
    fcntl(irc.fd, F_SETFL, O_NONBLOCK);


    userInput = "hello";
    int sendResult = send(irc.fd, userInput.c_str(), userInput.length()+1, 0);
    if (sendResult != -1)
    memset(buf, 0, 4096);
    int rb = recv(irc.fd, buf, 4096, 0);
    if (rb > 0)
        std::cout << std::string(buf, 0, rb);

    while (work) {
        std::cout << "> ";
        getline(std::cin, userInput);

        userInput += "\r\n";

        if (userInput.size() > 0) {
            int sendResult = send(irc.fd, userInput.c_str(), userInput.length()+1, 0);

            memset(buf, 0, 4096);
            int rb = recv(irc.fd, buf, 4096, 0);
            if (rb > 0)
                std::cout << std::string(buf, 0, rb);
            }
    }



//    bot.initIrc();
//    bot.initApi();
//
//    bot.authIrc();
//    while (work) {
//    }
//    bot.getApi();
}

//int main() {
//
//
//    int apiSocket = socket(AF_INET, SOCK_STREAM, 0);
//    sockaddr_in apiAddr;
//    memset(&apiAddr, 0, sizeof(apiAddr));
//    apiAddr.sin_family = AF_INET;
//
//
//    apiAddr.sin_family = AF_INET;
//    apiAddr.sin_port = htons(80);
//    apiAddr.sin_addr.s_addr = inet_addr("188.166.16.132");
//
//    if (apiAddr.sin_addr.s_addr == static_cast<in_addr_t>(-1)) {
//        std::perror("inet");
//        exit(1);
//    }
//
//    if (connect(apiSocket, (struct sockaddr*)&apiAddr, sizeof(apiAddr)) < 0) {
//        std::perror("connect");
//    }
//
//    std::stringstream ss;
//
//    ss << "GET /data/2.5/weather?lat=44.34&lon=10.99&appid=0162fdab085f4b6ab262e76ffc309667 HTTP/1.1\r\n"
//          "Host: api.openweathermap.org\r\n"
//          "Accept: application/json\r\n"
//          "Connection: close\r\n" << "\r\n";
//
//    if (send(apiSocket, ss.str().c_str(), ss.str().length(), 0) < 0) {
//        std::perror("send");
//    }
//
//    char buf[4096] = {0};
//    int rd = read(apiSocket, buf, 4096);
//
//    if (rd < 0)
//        std::perror("read");
//
//    std::cout << std::string(buf) << std::endl;
//
//    return 0;
//}



