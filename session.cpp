
#include "session.hpp"

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
	std::cout << "session: handle" << std::endl;
	if (r)
	{
		int rc = read(GetFd(), buffer, sizeof(buffer));
		if (rc > 0)
		{
			the_master->sendAll(buffer, this);
		}
		else if (rc == 0)
		{
			the_master->RemoveSession(this);
			return ;
		}
//		std::cout << "\n^^^^^^^\n" << buffer << "\nnumber of byte: " << rc << "\n________" << std::endl;
	}
}

void Session::send(const char *msg)
{
	write(GetFd(), msg, strlen(msg));
}

