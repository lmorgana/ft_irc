
#include "header.hpp"
#include "server.hpp"
#include "sockets.hpp"

static int port = 7777;

int main()
{
	// EventSelector *selector = new EventSelector;
	// Server *serv = Server::Start(selector, port);
	// if(!serv) {
	// 	perror("server");
	// 	return 1;
	// }
	// selector->Run();
	// return 0;

	std::string str = "lol kek\r\n";
	str = str.substr(0, str.length() - 2);
	std::cout << str;
}
