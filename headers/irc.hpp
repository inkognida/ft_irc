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
# include "User.hpp"
# include "Channel.hpp"
# include "Server.hpp"
# include <fcntl.h>


bool                        correctName(std::string name);
std::vector<std::string>    splitChannelArgs(std::string &names);