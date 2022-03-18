#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_PORT 10000
#define MIN_PORT 1023

class Bot
{
private:
	int			fd;
	int			port;
	std::string name;

private:
	void registration ();

public:
	Bot() : fd(0), port(0), name("bot") {};
	Bot(const std::string &name, int port) : port(port), name(name) {};
	~Bot() {};

	bool start();
	bool run_connect();
};

#endif
