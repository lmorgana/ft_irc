#include "Channel.hpp"

Channel::Channel(std::string name)
{
    this->_name = name;
}

std::string Channel::getName()
{
    return _name;
}

Client * Channel::getHostChannel()
{
    if (clients.size())
        return clients[0];
    return NULL;
}
std::vector<Client *> Channel::getClients()
{
    return clients;
}
void Channel::kickClient(Client *client)
{
    size_t size = clients.size();
    int index = -1;
    for (size_t i = 0; i < size; i++)
    {
        if (clients[i] == client)
            index = i;
    }
    if (index != -1)
        clients.erase(clients.begin() + index);
}

void Channel::addClient(Client *client)
{
    size_t size = clients.size();
    bool flag = true;
    for (size_t i = 0; i < size; i++)
    {
        if (clients[i] == client)
            flag = false;
    }
    if (flag)
        clients.push_back(client);
}

 Channel::~Channel()
 {}