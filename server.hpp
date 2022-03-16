#ifndef SERVER_HPP
#define SERVER_HPP

#include "header.hpp"
#include "sockets.hpp"
#include "session.hpp"
#include "Book.hpp"

class Session;

class Server : public FdHandler {
	EventSelector	*the_selector;
	struct item {
		Session *s;
		item *next;
	};
	item *first;
	Book book;

	Server(EventSelector *sel, int fd, char *pass);
public:
	~Server();

	static Server *Start(EventSelector *sel, int port, char *pass);

	void RemoveSession(Session *s, const char *msg);
	Book *getBook();

private:
	virtual void Handle(bool r);
};

#endif
