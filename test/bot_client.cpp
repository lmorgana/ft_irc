
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <map>
#include <unistd.h>

int main (int argc, char **argv)
{
	(void) argc;

	int port = int (atoi(argv[2]));
	char *bot_name = argv[1];
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
		std::cout << "ANTON here" << std::endl;
		write(ls, "PASS PASS\n", 10);
		sleep(1);
		write(ls, "NICK \n", 5);
		write(ls, bot_name, strlen(bot_name));
		write(ls, "\n", 1);
		sleep(1);
		write(ls, "USER BOT_", 9);
		write(ls, bot_name, strlen(bot_name));
		write(ls, "localhost serv: R", 16);
		write(ls, bot_name, strlen(bot_name));
		write(ls, "\n", 1);
	}
//	close(ls);
	return (0);
}
