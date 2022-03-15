#ifndef SERVER_HPP
#define SERVER_HPP

#include "header.hpp"
#include "sockets.hpp"
#include "session.hpp"

class Session;

class Server : public FdHandler {
	EventSelector *the_selector;
	struct item {
		Session *s;
		item *next;
	};
	item *first;

	Server(EventSelector *sel, int fd);
public:
	~Server();

	static Server *Start(EventSelector *sel, int port);

	void RemoveSession(Session *s);
	void sendAll(const char *msg, Session *except);


private:
	virtual void Handle(bool r);
};

#endif
