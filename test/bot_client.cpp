
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <map>
#include <unistd.h>

void regestration (int fd, std::string name)
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

int main (int argc, char **argv)
{
	(void) argc;

	int port = int (atoi(argv[2]));
	std::string bot_name = argv[1];
	int ls, opt, res;
	struct sockaddr_in addr, serv_addr;

	ls = socket(AF_INET, SOCK_STREAM, 0);
	if(ls == -1)
		return (1);

	opt = 1;
	setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	res = bind(ls, (struct sockaddr*) &addr, sizeof(addr));
	if(res == -1)
	{
		std::cout << "bind error" << std::endl;
		return (1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(6667);
	res = connect(ls,( struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(res == -1)
	{
		std::cout << "connect error" << std::endl;
		return (1);
	}
	else
	{
		regestration(ls, bot_name);
	}
	sleep(3);
	close(ls);
	return (0);
}
