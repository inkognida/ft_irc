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

// bot
class Bot {
public:
    Bot();
    ~Bot();

    void    init(srv&);

    void    authIrc();
    void    getIrcReq();

    std::string    getApiReq(std::string, std::string);
    //tools
    void            hostnameIP(std::string);
    void            err(std::string);
    void            sigHand(int);
    std::string     recvFunc(int);

    void    initAPI();


    void    toSend(srv&, std::string);

    //getters
    srv&    getApi(void) { return this->api; };
    srv&    getIrc(void) { return this->irc; };

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

void    Bot::init(srv &srvinfo) {
    srvinfo.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (srvinfo.fd < 0)
        err("Failed to create socket");

    srvinfo.addr.sin_family = AF_INET;
    srvinfo.addr.sin_port = htons(srvinfo.port);

    // TODO check this section
    hostnameIP(srvinfo.hostname);
    srvinfo.addr.sin_addr.s_addr = inet_addr(ip);
    memset(ip, 0, 4096);

    if (connect(srvinfo.fd, (struct sockaddr*)&srvinfo.addr, sizeof(srvinfo.addr)) < 0)
        err("Failed to connect");

    // TODO ANALYZE
    if (srvinfo.port == 6667) {
        fcntl(srvinfo.fd, F_SETFL, O_NONBLOCK);
    }
}

//void    Bot::initAPI() {
//    api.fd = socket(AF_INET, SOCK_STREAM, 0);
//
//
//    if (irc.fd < 0)
//        exit(1);
//    memset(&api.addr, 0, sizeof(api.addr));
//
//    api.addr.sin_family = AF_INET;
//    api.addr.sin_port = htons(80);
//    api.addr.sin_addr.s_addr = inet_addr("37.139.1.159");
//
//
//    if (connect(api.fd, (struct sockaddr*)&api.addr, sizeof(api.addr)) < 0){
//        std::cout << "connect\n";
//        exit(1);
//    }
//}

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
        strcpy(ip, inet_ntoa(h->sin_addr));
    }

    freeaddrinfo(servinfo);
}

std::string    Bot::getApiReq(std::string lat, std::string lon) {
    std::stringstream  in;
    in << "GET /data/2.5/weather?lat=" << "64.34" << "&lon=" << "10.99" << "&appid=" << "0162fdab085f4b6ab262e76ffc309667" << " HTTP/1.1\r\n"
       << "Host: api.openweathermap.org\r\n" << "Accept: application/json\r\n" << "Connection: close\r\n" << "\r\n";


    if (send(getApi().fd, in.str().c_str(), in.str().length(), 0) < 0)
        err("Failed to send api");

    char buf[4096];

    int rd = recv(getApi().fd, buf, 4096, 0);
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

    return apiResponse;
}

std::string    Bot::recvFunc(int fd) {
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

void    Bot::toSend(srv &srvinfo, std::string in) {
    if (send(irc.fd, in.c_str(), in.length(), 0) < 0)
        err("Failed to send to irc");
}

void    Bot::authIrc() {
    std::string userInput;
    for (int i = 0; i < 1; i++) { // TODO use i < 3
        getline(std::cin, userInput);
        if (send(irc.fd, userInput.c_str(), userInput.length() + 1, 0) < 0)
            exit(1);
    }
}

bool work = true;
void Bot::sigHand(int num) {
    (void)num;
    work = false;
}


std::string gg(int fd) {
    std::stringstream in;
    char buf[2048] = {0};
    int rd;

    in << "GET /data/2.5/weather?lat=" << "66.34" << "&lon=" << "10.99" << "&appid=" << "0162fdab085f4b6ab262e76ffc309667" << " HTTP/1.1\r\n"
       << "Host: api.openweathermap.org\r\n" << "Accept: application/json\r\n" << "Connection: close\r\n" << "\r\n";

    if (send(fd, in.str().c_str(), in.str().length(), 0) < 0) {
        std::cout << std::strerror(errno);
    }

    rd = read(fd, buf, 2047);
    std::cout << std::string(buf) << std::endl;
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
    return apiResponse;
}


int main() {
    Bot bot;
    bot.init(bot.getIrc());
//    bot.init(bot.getApi());


    srv irc;
    irc.fd = socket(AF_INET, SOCK_STREAM, 0);


    if (irc.fd < 0)
        exit(1);
    memset(&irc.addr, 0, sizeof(irc.addr));

    irc.addr.sin_family = AF_INET;
    irc.addr.sin_port = htons(80);
    irc.addr.sin_addr.s_addr = inet_addr("37.139.1.159");


    if (connect(irc.fd, (struct sockaddr*)&irc.addr, sizeof(irc.addr)) < 0){
        std::cout << "connect\n";
        exit(1);
    }

    bot.authIrc();

    std::string r = gg(irc.fd);
    std::string r1 = gg(irc.fd);

    std::cout << r << std::endl;
    std::cout << r1 << std::endl;

//    int i = 0;
//    while (work && i < 10) {
//        std::string tmp = bot.recvFunc(bot.getIrc().fd);
////        std::string response = "PRIVMSG a " + gg(irc.fd);
//        i++;
//
////        if (tmp.length() != 0 && tmp.find(":PUSSY") != std::string::npos) {
////            std::string response = "PRIVMSG a " + gg(irc.fd);
////            int s = send(bot.getIrc().fd, response.c_str(), response.length()+1, 0);
////            if (s < 0)
////                exit(1);
////        }
//
////        if (tmp.find(":PUSSY") != std::string::npos)
////        int s = send(bot.getIrc().fd, response.c_str(), response.length()+1, 0);
////        if (s < 0)
////            exit(1);
//    }

//    bot.init(bot.getApi());

//    std::stringstream in;


//    in << "GET /data/2.5/weather?lat=" << "66.34" << "&lon=" << "10.99" << "&appid=" << "0162fdab085f4b6ab262e76ffc309667" << " HTTP/1.1\r\n"
//       << "Host: api.openweathermap.org\r\n" << "Accept: application/json\r\n" << "Connection: close\r\n" << "\r\n";
//
//
//    if (send(bot.getApi().fd, in.str().c_str(), in.str().length(), 0) < 0)
//        exit(1);
//
//    char buf[4096];
//
//    int rd = recv(bot.getApi().fd, buf, 4096, 0);
//    std::string tmp(buf);
//
//    std::cout << tmp;
//
//    std::string apiResponse;
//
//    size_t found = tmp.find("name");
//    if (found != std::string::npos) {
//        found += std::string("name: ").length();
//        while (tmp[found] != ',') {
//            apiResponse += tmp[found];
//            found++;
//        }
//        apiResponse += "\n";
//    } else
//        apiResponse = "Something went wrong\n";
    return 0;
}


void main2() {
    srv irc;
    irc.fd = socket(AF_INET, SOCK_STREAM, 0);


    if (irc.fd < 0)
        exit(1);
    memset(&irc.addr, 0, sizeof(irc.addr));

    irc.addr.sin_family = AF_INET;
    irc.addr.sin_port = htons(80);
    irc.addr.sin_addr.s_addr = inet_addr("37.139.1.159");


    if (connect(irc.fd, (struct sockaddr*)&irc.addr, sizeof(irc.addr)) < 0){
        std::cout << "connect\n";
        exit(1);
    }

    std::stringstream in;


    in << "GET /data/2.5/weather?lat=" << "66.34" << "&lon=" << "10.99" << "&appid=" << "0162fdab085f4b6ab262e76ffc309667" << " HTTP/1.1\r\n"
    << "Host: api.openweathermap.org\r\n" << "Accept: application/json\r\n" << "Connection: close\r\n" << "\r\n";


    if (send(irc.fd, in.str().c_str(), in.str().length(), 0) < 0)
        exit(1);

    char buf[4096];

    int rd = recv(irc.fd, buf, 4096, 0);
    std::string tmp(buf);

    std::cout << tmp;

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

//
//
//    //auth -> pass, nick, user
//    std::string userInput;
//    for (int i = 0; i < 1; i++) { // TODO use i < 3
//        getline(std::cin, userInput);
//        if (send(irc.fd, userInput.c_str(), userInput.length()+1, 0) < 0)
//            exit(1);
//    }
//
//    std::string response;
//    while (work) {
//        std::string tmp; // = recvFunc(irc.fd);
//        if (tmp.find(":PUSSY") != std::string::npos) {
//            if (send(irc.fd, response.c_str(), response.length()+1, 0) < 0)
//                exit(1);
//            continue;
//        }
//    }

    //    char buf[4096];
//    int rd = recv(irc.fd, buf, 4096, 0);
//    std::cout << std::string(buf);
//    memset(buf, 0, 4096);
//
//    std::stringstream input;
//    input << "PASS 123\n";
//
//    if (send(irc.fd, input.str().c_str(), input.str().length(), 0) < 0)
//        exit(1);
//
//    std::stringstream nick;
//    nick << "NAMES\n";
//    if (send(irc.fd, nick.str().c_str(), nick.str().length(), 0) < 0)
//        exit(1);
//
//    rd = recv(irc.fd, buf, 4096, 0);
//    std::cout << std::string(buf);
//    std::cout << rd << std::string(buf);
//    memset(buf, 0, 4096);
//
//    std::stringstream input;
//    input << "NAMES\n";
//
//    size_t s = send(irc.fd, input.str().c_str(), input.str().length(), 0);
//    std::cout << s << std::endl;
//    recvFunc(irc.fd);

//
//    rd = recv(irc.fd, buf, 4096, 0);
//    std::cout << rd << std::string(buf);


//    return 0;


//    userInput = "hello";
//    int sendResult = send(irc.fd, userInput.c_str(), userInput.length()+1, 0);
//    if (sendResult != -1)
//    memset(buf, 0, 4096);
//    int rb = recv(irc.fd, buf, 4096, 0);
//    if (rb > 0)
//        std::cout << std::string(buf, 0, rb);
//
//    std::string userInput;
//    while (work) {
//        std::cout << "> ";
//        getline(std::cin, userInput);
//
//        userInput += "\r\n";
//
//        if (userInput.size() > 0) {
//            int sendResult = send(irc.fd, userInput.c_str(), userInput.length()+1, 0);
//
//            memset(buf, 0, 4096);
//            int rb = recv(irc.fd, buf, 4096, 0);
//            if (rb > 0)
//                std::cout << std::string(buf, 0, rb);
//            }
//    }

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



