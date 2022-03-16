
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
	}
}

void Session::send(char *msg)
{
	if (msg)
		write(GetFd(), msg, sizeof(msg));
}
