
#include "sockets.hpp"

template <class T>
void ft_print_vector(std::vector<T> vector)
{
	int i = 0;

	std::cout << "start print" << std::endl;
	for (typename std::vector<T>::iterator iter = vector.begin(); iter != vector.end(); iter++, i++)
		std::cout << "index " << i << ", value " << *iter << std::endl;
	std::cout << "end print" << std::endl;
}

void EventSelector::Add(FdHandler *h)
{
	int fd = h->GetFd();
	pollfd plf;

	if (fd_array.empty() || int(fd_array.size()) < fd + 1)
		fd_array.resize(fd + 1);
	plf.fd = fd;
	plf.events = POLLIN;
	if (search_fd_poolfd(fd) == -1)
		poll_array.push_back(plf);
	if(fd > max_fd)
		max_fd = fd;
	fd_array[fd] = h;
}

bool EventSelector::Remove(FdHandler *h)
{
	int fd = h->GetFd();

	if(fd >= int(fd_array.size()) || fd_array[fd] != h)
		return false;
	fd_array[fd] = nullptr;

	iterator_pollfd iter;
	for (iter = poll_array.begin(); iter != poll_array.end(); iter++)
	{
		if (iter->fd == fd)
			break;
	}
	if (iter == poll_array.end())
		return false;
	poll_array.erase(iter);
	close(fd);
	if(fd == max_fd)
	{
		while(max_fd >= 0 && !fd_array[max_fd])
			max_fd--;
	}
	return true;
}

int	EventSelector::search_fd_poolfd(int fd)
{
	if (fd > 2)
	{
		for (int i = 0; i < int(poll_array.size()); i++)
			if (poll_array[i].fd == fd)
				return (i);
	}
	return (-1);
}

void EventSelector::Run()
{
	int index = -1;
	quit_flag = false;

	for (int i = 0; i <= max_fd; i++)
	{
		if (fd_array[i])
		{

			index = search_fd_poolfd(fd_array[i]->GetFd());
			poll_array[index].events = POLLIN;
		}
	}
	do
	{
		iterator_pollfd iter = poll_array.begin();
		int res = poll(iter.base(), poll_array.size(), 10000);
		if (res < 0)
		{
			if (errno == EINTR)
				continue;
			else
				break;
		}
		if (res > 0)
		{
			for (int i = 0; (i <= max_fd && res > 0); i++)
			{
				if(!fd_array[i])
					continue;
				index = search_fd_poolfd(i);
				if (poll_array[index].revents & POLLIN)
				{
					poll_array[index].revents = 0;
					fd_array[i]->Handle(true);
					res--;
				}
				if (poll_array[index].revents & POLLOUT)
				{
					poll_array[index].revents = 0;
					fd_array[i]->Handle(false);
					res--;
				}
			}
		}
//		std::cout << "nothing happen" << std::endl;
	} while (!quit_flag);
}

FdHandler::~FdHandler()
{
	if(own_fd)
		close(fd);
}
