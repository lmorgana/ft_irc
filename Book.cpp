
   
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
    {
        delete clients[i];
    }
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