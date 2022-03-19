#include "Channel.hpp"

Channel::Channel(std::string name)
{
    this->_name = name;
    clients = new std::vector<Client *>;
}

std::string Channel::getName()
{
    return _name;
}

Client * Channel::getHostChannel()
{
    if (clients->size())
        return (*clients)[0];
    return NULL;
}

std::vector<Client *> * Channel::getClients()
{
    return clients;
}

bool Channel::kickClient(Client *client)
{
    size_t size = clients->size();
    int index = -1;

    for (size_t i = 0; i < size; i++)
    {
        if ((*clients)[i] == client)
            index = i;
    }
    if (index != -1)
    {
        clients->erase(clients->begin() + index);
        return true;
    }
    return false;
}

void Channel::addClient(Client *client)
{
    size_t size = clients->size();
    bool flag = true;

    for (size_t i = 0; i < size; i++)
    {
        if ((*clients)[i] == client)
            flag = false;
    }
    if (flag)
        clients->push_back(client);
}

bool Channel::searchClient(Client *client)
{
    size_t size = clients->size();
    for (size_t i = 0; i < size; i++)
    {
        if ((*clients)[i] == client)
            return true;
    }
    return false;
}

 bool Channel::searchNick(std::string nick)
 {
    size_t size = clients->size();

    for (size_t i = 0; i < size; i++)
    {
        if ((*clients)[i]->getNick() == nick)
            return true;
    }
    return false;
 }

 Channel::~Channel()
 {
     delete clients;
 }