
#include "bot.hpp"

void Bot::registration ()
{
	std::cout << name << " registrating" << std::endl;
	write(fd, "PASS PASS\n", 10);
	sleep(1);
	std::string st1 = "NICK " + name + " \n";
	write(fd, st1.c_str(), st1.length());
	sleep(1);
	st1.clear();
	st1 = "USER BOT_" + name + " localhost serv: R" + name + "\n";
	write(fd, st1.c_str(), st1.length());
}

bool Bot::run_connect()
{
	int opt, res;
	struct sockaddr_in addr, serv_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1)
		return (false);

	opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(6669);
	res = bind(fd, (struct sockaddr*) &addr, sizeof(addr));
	std::cout << errno << std::endl;
	if(res == -1)
	{
		std::cout << "bind error" << std::endl;
		return (false);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	res = connect(fd,( struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(res == -1)
	{
		std::cout << "connect error" << std::endl;
		return (false);
	}
	return (true);
}

bool Bot::start()
{
	if (run_connect())
	{
		std::cout << name << " on server" << std::endl;
		registration();
		return (true);
	}
	std::cout << "something wrong" << std::endl;
	return (false);
}

