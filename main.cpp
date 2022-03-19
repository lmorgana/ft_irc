
#include "header.hpp"
#include "sockets.hpp"
#include "server.hpp"


#define MAX_PORT 49151
#define MIN_PORT 1023

bool is_digit(char *str)
{
	if (!str)
		return (false);
	for (size_t i = 0; str[i] != '\0'; i++)
	{
		if (isdigit(!str[i]))
			return (false);
	}
	return (true);
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments" << std::endl;
		return (1);
	}
	if (!is_digit(argv[1]))
	{
		std::cout << "Port must be number" << std::endl;
		return (1);
	}
	int port = atoi(argv[1]);
	if (port <= MIN_PORT || port >= MAX_PORT)
	{
		std::cout << "Wrong port number"<< std::endl;
		return (1);
	}
	EventSelector *selector = new EventSelector;
	Server *serv = Server::Start(selector, port, argv[2]);
	if(!serv) {
		perror("server");
		return 1;
	}
	selector->Run();
	return 0;
}
