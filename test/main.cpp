
#include "bot.hpp"

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

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments" << std::endl;
		return (1);
	}
	if (!is_digit(argv[2]))
	{
		std::cout << "Port must be number" << std::endl;
		return (1);
	}
	int port = atoi(argv[2]);
	if (port <= MIN_PORT || port >= MAX_PORT)
	{
		std::cout << "Wrong port number"<< std::endl;
		return (1);
	}
	Bot our_bot (argv[1], port);
	return (our_bot.start());
}
