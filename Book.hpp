#ifndef BOOK_HPP
#define BOOK_HPP

#include "header.hpp"
#include "Client.hpp"
#include <vector>
#include "Channel.hpp"

class Session;
class Channel;

class Book{
	
	private:

    std::vector<Client *> clients;
    std::vector<Channel *> channels; 
    std::string _password;
    public:

    Book(char *password);
    Client *getClient(Session *session);
    Session * searchSession(std::string nick);
    bool searchBySession(Session *session);
    std::string getPassword();
    bool checkNicknames(std::string nick);
    size_t size();
    bool searchChannel(std::string name);
    bool kickClient(Session *session);

    void joinClientChannel(std::string name, Client *client);
    bool kickClientChannel(std::string name, Client *client);
    std::vector<Client *> * getClientsChannel(std::string name); 

    ~Book();
	
};
 
#endif