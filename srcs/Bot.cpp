#include "../headers/irc.hpp"

class Socket {

public:
    Socket(std::string hostname, int port, std::string token = "") {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0)
            err("Failed to create socket");

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        hostname_to_ip(hostname);
        addr.sin_addr.s_addr = inet_addr(ip);

        if (addr.sin_addr.s_addr == static_cast<in_addr_t>(-1))
            err("Failed to inet_addr");

    };

    ~Socket(void) {
        close(fd);
    };

    void             hostname_to_ip(std::string hostname) {
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

    void            toConnect(void) {
        if (connect(fd, (struct sockaddr*)addr, sizeof(addr)) < 0)
            err("Failed to connect");
    }

    int             toSend(std::string request) {
        if (send(fd, request.c_str(), request.length(), 0) < 0)
            err("Failed to send");
    }

    std::string     toRecv(void) {
        // TODO accept JSON from API

        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));

        int rd = read(fd, buf, 4096);

        if (rd < 0)
            err("Failed to recv");

    }



    int             getFd(void) { return this->fd; };

    // tools
    void            err(std::string reason) {
        std::cout << reason << std::endl;
        exit(1);
    }
    int             ping(std::string hostIP) {
        std::string tmp = "ping -c -s1 " + hostIP + " > /dev/null 2>&1";
        int x = std::system(tmp.c_str());

        if (x == 0)
            return (1);
        else
            return (0);
    }

private:
    int             fd;

    char            ip[4096];
    int             port;
    std::string     token;

    sockaddr_in     addr;
};








class Bot {

public:
    Bot();
    ~Bot();


    void                    createConnection();


    std::string              requestAPI(std::string lat, std::string lon) {
        std::stringstream req;

        req << "GET /data/2.5/weather?lat=" << lat << "&lon=" << lon << "&appid=" << tokenAPI << " HTTP/1.1\r\n"
              "Host: api.openweathermap.org\r\n"
              "Accept: application/json\r\n"
              "Connection: close\r\n" << "\r\n";

        socketAPI->toSend(req.str());
        responseAPI = socketAPI->toRecv()
    }


    void            err(std::string reason) {
        std::cout << "Error: " << reason << std::endl;
        exit(1);
    };

    // tools

private:
    Socket          *socketIRC;
    Socket          *socketAPI;

    std::string     hostnameIRC;
    std::string     hostnameAPI;

    std::string     portIRC;
    std::string     portAPI;

    // NO TOKEN for IRC
    std::string     tokenAPI;

    std::string     responseAPI;
};

void            Bot::createConnection() {
    socketIRC = new Socket(hostnameIRC, portIRC);
    socketAPI = new Socket(hostnameAPI, portAPI, tokenAPI);

    socketIRC->toConnect();
    socketAPI->toConnect();

    fcntl(socketIRC->getFd(), F_SETFL, O_NONBLOCK);
    fcntl(socketAPI->getFd(), F_SETFL, O_NONBLOCK);
}

Bot::Bot() {
    this->hostnameIRC = "localhost";
    this->portIRC = 6667;

    this->hostnameAPI = "api.openweathermap.org";
    this->portAPI = 80;
    this->tokenAPI = "0162fdab085f4b6ab262e76ffc309667";
}

Bot::~Bot() {
    // TODO check memory leaks
    delete socketAPI;
    delete socketIRC;
}

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

int main() {

    int p = ping("1.2.1241");
    if (p)
        std::cout << "SUCESS";
    else
        std::cout << "Fail";

    int apiSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in apiAddr;
    memset(&apiAddr, 0, sizeof(apiAddr));
    apiAddr.sin_family = AF_INET;


    apiAddr.sin_family = AF_INET;
    apiAddr.sin_port = htons(80);
    apiAddr.sin_addr.s_addr = inet_addr("188.166.16.132");

    if (apiAddr.sin_addr.s_addr == static_cast<in_addr_t>(-1)) {
        std::perror("inet");
        exit(1);
    }

    if (connect(apiSocket, (struct sockaddr*)&apiAddr, sizeof(apiAddr)) < 0) {
        std::perror("connect");
    }

    std::stringstream ss;

    ss << "GET /data/2.5/weather?lat=44.34&lon=10.99&appid=0162fdab085f4b6ab262e76ffc309667 HTTP/1.1\r\n"
          "Host: api.openweathermap.org\r\n"
          "Accept: application/json\r\n"
          "Connection: close\r\n" << "\r\n";

    if (send(apiSocket, ss.str().c_str(), ss.str().length(), 0) < 0) {
        std::perror("send");
    }

    char buf[4096] = {0};
    int rd = read(apiSocket, buf, 4096);

    if (rd < 0)
        std::perror("read");

    std::cout << std::string(buf) << std::endl;

    return 0;
}
