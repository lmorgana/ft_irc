
#include "Book.hpp"

Book::Book(char *password)
{
    this->_password = password;
}

Client  * Book::getClient(Session *session)
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

bool Book::kickClientChannel(std::string name, std::string nick)
{
    Client *client = NULL;
    size_t  size = clients.size();

    for (size_t i = 0; i < size; i++)
    {
       if (clients[i]->getNick() == nick)
           client = clients[i];
    }
    
    size = channels.size();
    for(size_t i = 0; i < size; i++)
    {
        if (name == channels[i]->getName())
            return channels[i]->kickClient(client);
    }
    return false;
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

bool Book::searchChannel(std::string name)
{
    size_t size = channels.size();

    for(size_t i = 0; i < size; i++)
    {
        if (name == channels[i]->getName())
            return true;
    }
    return false;
}

bool Book::kickClient(Session *session)
{
    size_t size = channels.size();
    int index = -1;
    Client * client = getClient(session);

    for(size_t i = 0; i < size; i++)
    {
        if (channels[i]->searchClient(client))
            channels[i]->kickClient(client);
    }
    size = clients.size();
    for(size_t i = 0; i < size; i++)
    {
        if (clients[i] == client)
            index = i;
    }
    if (index != -1)
    {
        clients.erase(clients.begin() + index);
        delete client;
        return true;
    }
    return false;
}

bool Book::checkNickInChannels(std::string nick, std::vector<std::string> name_channel)
{
    size_t size_chanel =  channels.size();
    size_t size_name_channel_vec = name_channel.size();

    for (size_t i = 0; i < size_chanel; i++)
    {
        for(size_t j = 0; j < size_name_channel_vec; j++)
        {
            if (channels[i]->getName() == name_channel[j])
            {
                if (channels[i]->searchNick(nick))
                    return true;
            }
        }
    }
    return false;
}

Session * Book::getSession(std::string nick)
{
    size_t size = clients.size();

    for (size_t i = 0; i < size; i++)
    {
        if (clients[i]->getNick() == nick)
            return clients[i]->getSession();
    }
    return NULL;
}

bool Book::checkHostChanel(std::string name, Client *client)
{
    size_t size = channels.size();
    Client *ck;

    for (size_t i = 0; i < size; i++)
    {
        if (channels[i]->getName() == name)
        {
            ck = channels[i]->getHostChannel();
            if (ck && ck == client)
                return true;
        }
    }
    return false;
}

bool Book::checkChannel(std::string name)
{
    size_t size = channels.size();
    for (size_t i = 0; i < size; i++)
    {
        if (channels[i]->getName() == name)
            return true;
    }
    return false;
}

std::vector<std::string> Book::getNickChanel(std::string name)
{
    std::vector<std::string> vec;
    std::vector<Client *> * vec_clients;

    vec_clients = getClientsChannel(name);
    if (!vec_clients)
        return vec;
        
    size_t size = (*vec_clients).size();
    for (size_t i = 0; i < size; i++)
    {
        vec.push_back((*vec_clients)[i]->getNick());
    }
    return vec;
}

std::vector<Session *> Book::getAllSession()
{
    std::vector<Session *> session;

    size_t size = clients.size();
    for (size_t i = 0; i < size; i++)
        session.push_back(clients[i]->getSession());
    return session;
}

std::vector<Client *> Book::getAllClients()
{
    return clients;
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
