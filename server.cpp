
#include "server.hpp"

Server::Server(EventSelector *sel, int fd, char *pass)
		: FdHandler(fd, true), the_selector(sel), first(0), book(pass)
{
	the_selector->Add(this);
}

Server::~Server()
{
	while(first) {
		item *tmp = first;
		first = first->next;
		the_selector->Remove(tmp->s);
		delete tmp->s;
		delete tmp;
	}
	the_selector->Remove(this);
}

Server *Server::Start(EventSelector *sel, int port, char *pass)
{
	int ls, opt, res;
	struct sockaddr_in addr;

	ls = socket(AF_INET, SOCK_STREAM, 0);
	if(ls == -1)
		return (nullptr);

	opt = 1;
	setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	res = bind(ls, (struct sockaddr*) &addr, sizeof(addr));
	if(res == -1)
		return (nullptr);

	res = listen(ls, qlen_for_listen);
	if(res == -1)
		return (nullptr);
	std::cout << "Hello, server is here" << std::endl;
	return new Server(sel, ls, pass);
}

void Server::RemoveSession(Session *s, const char *msg)
{
	std::cout << msg << std::endl;
	the_selector->Remove(s);
	book.kickClient(s);
	item **p;
	for(p = &first; *p; p = &((*p)->next)) {
		if((*p)->s == s) {
			item *tmp = *p;
			*p = tmp->next;
			delete tmp->s;
			delete tmp;
			return;
		}
	}
}

void Server::Handle(bool r)
{
	if(!r)
		return;
	int sd;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	sd = accept(GetFd(), (struct sockaddr*) &addr, &len);
	if(sd == -1)
		return;

	item *p = new item;
	p->next = first;
	p->s = new Session(this, sd);
	first = p;

	the_selector->Add(p->s);
	std::cout << "*** someone connected with us ***" << std::endl;
}

Book *Server::getBook()
{
	return (&book);
}

void Server::send_msg(std::vector<struct returnRes> *result)
{
	for (size_t i = 0; i < result->size(); i++)
	{
		Session *n = (*result)[i].users;
		n->send((*result)[i].msg.c_str());
	}
}
