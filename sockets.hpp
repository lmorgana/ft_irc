#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include "header.hpp"

class FdHandler {
	int fd;
	bool own_fd;

public:
	FdHandler(int a_fd, bool own = true) : fd(a_fd), own_fd(own) {}
	virtual ~FdHandler();
	virtual void Handle(bool r) = 0;

	int GetFd() const { return fd; }

	virtual bool WantRead() const { return true; }
	virtual bool WantWrite() const { return false; }
};

class EventSelector {
	typedef std::vector<FdHandler*>::iterator iterator_handler;
	typedef std::vector<pollfd>::iterator iterator_pollfd;

	std::vector<FdHandler*> fd_array;
	std::vector<pollfd> poll_array;
	int max_fd;

	bool quit_flag;
public:
	EventSelector() : fd_array(), quit_flag(false) {}
	~EventSelector() {};

	void Add(FdHandler *h);
	bool Remove(FdHandler *h);

	void BreakLoop() { quit_flag = true; }

	void Run();

private:
	int	search_fd_poolfd(int fd);
};

#endif
