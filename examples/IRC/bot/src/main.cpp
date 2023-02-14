#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/tcp.h>
#include <sys/types.h>
#include <netdb.h>
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
# include <fstream>
# include <iostream>
# include <fcntl.h>

bool	work = true;

void	sigHandler(int signum)
{
	(void)signum;
	work = false;
}



int ping(std::string hostIP) {
    std::string tmp = "ping -c -s1 " + hostIP + " > /dev/null 2>&1";
    int x = std::system(tmp.c_str());
    if (x == 0) {
        return (1);
    } else {
        return (0);
    }
}

//int hostname_to_ip(const char* host_name, char* ip)
//{
//    struct addrinfo hints, * servinfo, * p;
//    struct sockaddr_in* h;
//    int rv;
//
//    memset(&hints, 0, sizeof hints);
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//
//    if ((rv = getaddrinfo(host_name, "http", &hints, &servinfo)) != 0)
//    {
//        return 1;
//    }
//
//    for (p = servinfo; p != NULL; p = p->ai_next)
//    {
//
//        h = (struct sockaddr_in*) p->ai_addr;
//        strcpy(ip, inet_ntoa(h->sin_addr));
//    }
//
//    freeaddrinfo(servinfo);
//    return 0;
//}
//
//tcp_client_t::tcp_client_t(const char* host_name, const unsigned short server_port)
//        : socket_t(),
//          m_server_port(server_port)
//{
//
//    char server_ip[100];
//    //get ip address from hostname
//    std::cout << "tcp_client_t::tcp_client_t ABOBA" << std::endl;
//
//    hostname_to_ip(host_name, server_ip);
//
//    std::cout << std::string(server_ip) << std::endl;
//    m_server_ip = server_ip;
//}


//int tcp_client_t::connect()
//{
//    struct sockaddr_in server_addr; // server address
//
//    // create a stream socket using TCP
//    if ((m_sockfd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
//    {
//        std::cout << "socket error: " << std::endl;
//        return -1;
//    }
//
//    // construct the server address structure
//    memset(&server_addr, 0, sizeof(server_addr)); // zero out structure
//    server_addr.sin_family = AF_INET; // internet address family
//    if (inet_pton(AF_INET, m_server_ip.c_str(), &server_addr.sin_addr) <= 0) // server IP address
//    {
//        std::cout << "inet_pton error: " << strerror(errno) << std::endl;
//        return -1;
//    }
//    server_addr.sin_port = htons(m_server_port); // server port
//
//    // establish the connection to the server
//    if (::connect(m_sockfd, (struct sockaddr*) & server_addr, sizeof(server_addr)) < 0)
//    {
//        std::cout << "connect error: " << strerror(errno) << std::endl;
//        return -1;
//    }
//    return 0;
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
