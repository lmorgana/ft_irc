
#include "session.hpp"
#include "parser.hpp"
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

int get_line(int fd, char *buffer, int len_buffer)
{
	char ch;
	int i = 0;
	int rc;

	while ((rc = read(fd, &ch, 1)) > 0)
	{
		if (i < len_buffer && ch != '\n')
			buffer[i] = ch;
		else if (ch == '\n')
		{
			buffer[i] = ch;
			break;
		}
		i++;
	}
	return (rc);
}

void Session::Handle(bool r)
{
	std::vector<struct returnRes> *result = new std::vector<struct returnRes>;
	if (r)
	{
		int rc = get_line(GetFd(), buffer, sizeof(buffer));
		if (rc > 0)
		{
			std::cout << "buffer: \"" << buffer << "\"" << "strlen" << strlen(buffer) << std::endl;
			result = checkData(this, buffer, the_master->getBook(), result);
			the_master->send_msg(result);
		}
		else if (rc == 0)
		{
			the_master->RemoveSession(this, EXIT_MSG);
			delete result;
			return ;
		}
		else
		{	// -1 can't read
			the_master->RemoveSession(this, "*** server close session ***");
			delete result;
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
