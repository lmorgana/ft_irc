
#include "session.hpp"
#include "registration.hpp"
#include <cstring>


Session::Session(Server *a_master, int fd)
		: FdHandler(fd, true), stat(READING), buf_used(0), ignoring(false),
		  name(0), the_master(a_master)
		  {}

Session::~Session()
{
	if(name)
		delete[] name;
}

void Session::Handle(bool r)
{
	struct returnRes *result = new struct returnRes;
	if (r)
	{
		int rc = read(GetFd(), buffer, sizeof(buffer));
		if (rc > 0)
		{
//			result = checkData(this, buffer, the_master->getBook(), result);
//			result->users[0]->send(result->msg.c_str());
			std::cout << buffer << std::endl;
		}
		else if (rc == 0)
		{
			the_master->RemoveSession(this, EXIT_MSG);
			return ;
		}
		else
		{	// -1 can't read
			the_master->RemoveSession(this, "*** server close session ***");
			return ;
		}
		bzero(buffer, strlen(buffer));
		delete result;
	}
}

void Session::send(const char *msg)
{
	if (msg)
		write(GetFd(), msg, strlen(msg));
}
