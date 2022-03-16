#ifndef WEBSERV_SESSION_HPP
#define WEBSERV_SESSION_HPP

#include "header.hpp"
#include "sockets.hpp"
#include "server.hpp"

#define READING 0
#define WRITING 1
#define LIMIT_OF_BUFFER 3096

enum {
	max_line_length = 512,
	qlen_for_listen = 16
};

class Server;

class Session : FdHandler {
	friend class Server;

	int		stat;
	char	buffer[max_line_length+1];	//main buffer
	int		buf_used;
	char	*RSbuffer;					//reserve buffer
	bool	ignoring;

	char	*name;

	Server	*the_master;

	Session(Server *a_master, int fd);
	~Session();


	virtual void Handle(bool r);
	void send(const char *msg);
};


#endif
