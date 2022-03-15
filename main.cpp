
#include "header.hpp"
#include "server.hpp"
#include "sockets.hpp"

static int port = 6667;

int main()
{
	EventSelector *selector = new EventSelector;
	Server *serv = Server::Start(selector, port);
	if(!serv) {
		perror("server");
		return 1;
	}
	selector->Run();
	return 0;
}
