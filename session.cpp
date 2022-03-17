
#include "session.hpp"
#include "registration.hpp"
#include <cstring>


Session::Session(Server *a_master, int fd)
		: FdHandler(fd, true), stat(READING), buf_used(0), ignoring(false),
		  name(0), the_master(a_master)
{
	//Send("Your name please: ");
}

Session::~Session()
{
	if(name)
		delete[] name;
}

void Session::Handle(bool r)
{
	struct returnRes *result = new struct returnRes;
	std::cout << "session: handle" << std::endl;

	if (r)
	{
		int rc = read(GetFd(), buffer, sizeof(buffer));
		if (rc > 0)
		{
			result = checkData(this, buffer, the_master->getBook(), result);
			result->users[0]->send(result->msg.c_str());
		}
		else if (rc == 0)
		{
			the_master->RemoveSession(this, EXIT_MSG);
			return ;
		}
		else
		{	// -1 can't read
//			std::cout << "something wrong with session" << std::endl;
			the_master->RemoveSession(this, "*** server close session ***");
			return ;
		}
//		std::cout << "------\n" << buffer << "number of byte: " << rc << "\n------\n" << std::endl;
		bzero(buffer, strlen(buffer));
		delete result;
	}
}

void Session::send(const char *msg)
{
	if (msg)
		write(GetFd(), msg, strlen(msg));
}
