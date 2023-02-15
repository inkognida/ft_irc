#include "Bot.hpp"
#include "Message.hpp"
#include "utils.hpp"
#include <string>
#include <fstream>
#include <sstream>
Bot::Bot(const string &filename) 
: _configFile(filename), _configLoaded(false), _IRCsocket(0), _APIsocket(0), _isActive(false) {
	loadConfig();
}

Bot::~Bot() {
	delete _IRCsocket;
	delete _APIsocket;
}

Bot::Bot(const Bot &other) {
	*this = other;
}

Bot &Bot::operator=(const Bot &other) {
	if (this != &other) {
		_configFile = other._configFile;
		_configLoaded = other._configLoaded;

		_IRCatServerIP = other._IRCatServerIP;
		_IRCatServerPort = other._IRCatServerPort;
		_IRCatServerPass = other._IRCatServerPass;
		_username = other._username;
		_hostname = other._hostname;
		_servername = other._servername;
		_realname = other._realname;
		_nickname = other._nickname;
		_API_Protocol = other._API_Protocol;
		_API_IP = other._API_IP;
		_API_Port = other._API_Port;
	}
	return *this;
}

void Bot::loadConfig( void ) {


	_IRCatServerIP = "127.0.0.1";
	_IRCatServerPort = "6667";
	_IRCatServerPass = "123";
	_username = "where_is_bot";
	_hostname = "127.0.0.1";
	_servername = "PUSSY";
	_realname = "where_is_bot";
	_nickname = "wherebot";
	_API_Protocol = "http";
	_API_IP = "37.139.1.159";
	_API_Port = "80";
	_botname = "wherebot";

}

bool Bot::confLoaded( void ) {
	return _configLoaded;
}

void Bot::createSockets( void ) {
	int IRCport = static_cast<in_port_t>(strtol(_IRCatServerPort.c_str(), NULL, 10));
	int APIport = static_cast<in_port_t>(strtol(_API_Port.c_str(), NULL, 10));
		
	_IRCsocket = new Socket(_IRCatServerIP, IRCport);
	_APIsocket = new Socket(_API_IP, APIport);
		
	_IRCsocket->tryToConnect();
	_APIsocket->tryToConnect();
	
	fcntl(_IRCsocket->getFd(), F_SETFL, O_NONBLOCK);
	//fcntl(_APIsocket->getFd(), F_SETFL, O_NONBLOCK);

	_isActive = true;
}

void Bot::Auth(void) {

	string pass = "PASS " + _IRCatServerPass + "\n";
	string user = "USER " + _username + " " + _hostname + " " + _servername + " " + _realname  + "\n";
	string nick = "NICK " + _botname + "\n";
	
	_IRCsocket->tryToSend(pass);
	_IRCsocket->tryToSend(user);
	_IRCsocket->tryToSend(nick);

}

string Bot::receiveMessage() {
	string req = _IRCsocket->tryToRecv();

	if (req == "") {
		_isActive = false;
	} 
	return req; 
}


void Bot::parseMessage(const string &msg) {

	static string last = "";

	std::queue<std::string> msgQueue = split(last + msg, '\n', true);
	while (msgQueue.size() > 0) {
		if (msgQueue.front().find('\n') != std::string::npos) {
			Message parsedMsg(msgQueue.front());

			action(parsedMsg);
		} 
		else {
			last = msgQueue.front();
		}
		msgQueue.pop();	
	}
}

string Bot::requestAPI( const string &name) {
	std::stringstream ss;

	ss << "GET " << getLocationURL(name) << " HTTP/1.1\r\n";
	ss << "\n\n";
    
	std::cout << ss.str() << std::endl;
	_APIsocket->tryToSend(ss.str());
	
	char buf[2048] = {0};
	int res = read(_APIsocket->getFd(), buf, 2047);
	
	if (res != -1) {
		return string(buf);
	} else {
		return "";
	}
}

string Bot::parseAPIresponse( const string &res ) {
	
	int pos = res.find("\r\n\r\n");
	if (res == "")
		return "Internal error";
	
	return res.substr(pos + 4);
}

const string Bot::getSender(const string &prefix) {
	
	char sender[512] = {0};
	sscanf(prefix.c_str(), "%[^!]", sender);

	return string(sender);
}

void Bot::action( Message &m ) {

	if (m.getCommand() == "PRIVMSG") {

		string res = requestAPI(m.getParams()[1]);
		string body = parseAPIresponse(res);
		string sender = getSender(m.getPrefix());

		sendMessage("PRIVMSG " + sender + " :" + body + "\n");
	
	} else if (m.getCommand() == "PING") {

		sendMessage("PONG :" + m.getParams()[1]);
	} else {
		//ignore other
	}
}

bool Bot::isActive( void ) {
	return _isActive;
}

int Bot::sendMessage(const string &msg) {
	return _IRCsocket->tryToSend(msg);
}

const string Bot::getLocationURL(const string &name) {
	return "/users/" + name + "/location";
}
