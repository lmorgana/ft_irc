#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "header.hpp"
#include "Client.hpp"
#include <vector>

class Client;

class Channel
{

    private:

    std::string _name;
    std::vector<Client *> clients;

    public:

    Channel(std::string name);
    std::string getName();
    Client * getHostChannel();
    std::vector<Client *> getClients();
    void kickClient(Client *client);
    void addClient(Client *client);
    ~Channel();
};
#endif