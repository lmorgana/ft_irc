
#include "Book.hpp"

Book::Book(char *password)
{
    this->_password = password;
}


Client * Book::getClient(Session *session)
{
    if (searchBySession(session) == false)
        clients.push_back(new Client(session));
    size_t size = clients.size();
    for (size_t i = 0; i < size;i++)
    {
        if (session == clients[i]->getSession())
            return (clients[i]);
    }
    return NULL;
}
Session * Book::searchSession(std::string nick)
{
    size_t size = clients.size();
    for (size_t i = 0; i < size;i++)
    {
        if (nick == clients[i]->getNick())
            return clients[i]->getSession();
    }
    return NULL;
}

size_t Book::size()
{
    return clients.size();
}

bool Book::searchBySession(Session *session)
{
    size_t size = clients.size();
    for (size_t i = 0; i < size;i++)
    {
        if (session == clients[i]->getSession())
            return (true);
    }
    return (false);
}
Book::~Book()
{
    size_t size = clients.size();
    for (size_t i = 0; i < size;i++)
        delete clients[i];
    size = channels.size();
    for (size_t i = 0; i < size;i++)
        delete channels[i];
}

std::string Book:: getPassword()
{
    return (_password);
}

bool Book::checkNicknames(std::string nick)
{
   size_t size = clients.size();
    for (size_t i = 0; i < size;i++)
    {
        if (nick == clients[i]->getNick())
            return (true);
    }
    return (false);
}

void Book::joinClientChannel(std::string name, Client *client)
{
    size_t size = channels.size();
    bool flag = true;
    for(size_t i = 0; i < size; i++)
    {
        if (name == channels[i]->getName())
            flag = false;
    }
    if (flag)
        channels.push_back(new Channel(name));
    size = channels.size();
    for(size_t i = 0; i < size; i++)
    {
        if (name == channels[i]->getName())
            channels[i]->addClient(client);
    }
}
void Book::kickClientChannel(std::string name, Client *client)
{
    size_t size = channels.size();
    for(size_t i = 0; i < size; i++)
    {
        if (name == channels[i]->getName())
            channels[i]->kickClient(client);
    }
}
std::vector<Client *> * Book::getClientsChannel(std::string name)
{

    size_t size = channels.size();
    for(size_t i = 0; i < size; i++)
    {
        if (name == channels[i]->getName())
            return channels[i]->getClients();
    }
    return NULL;
}