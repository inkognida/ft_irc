#include "Bot.hpp"
#include <csignal>

bool	work = true;

void	sigHandler(int signum)
{
	(void)signum;
	work = false;
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <bot-config.json>" << std::endl;
		return 1;
	}

	string filename(argv[1]);

	signal(SIGINT, sigHandler);

	Bot bot(filename);
	

	bot.createSockets();
	bot.Auth();
	
	string msg;
	while (work)
	{
		msg = bot.receiveMessage();
		bot.parseMessage(msg);
	}
	return 0;
}
