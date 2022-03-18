
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
	std::vector<struct returnRes> *result = new std::vector<struct returnRes>;
	if (r)
	{
		int rc = read(GetFd(), buffer, sizeof(buffer));
		if (rc > 0)
		{
			result = checkData(this, buffer, the_master->getBook(), result);
//			the_master->send_msg(result->msg.c_str(), result->users);
			std::cout << (*result)[0].msg << std::endl;
			the_master->send_msg(result);
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
