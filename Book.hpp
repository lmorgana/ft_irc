#ifndef BOOK_HPP
#define BOOK_HPP

#include "header.hpp"
#include "Client.hpp"
#include <vector>

class Book{
	
	private:

    std::vector<Client *> clients;
    std::vector<Channel *> channels; 
    std::string _password;
    public:

    Book(char *password);
    Client *getClient(Session *session);
    bool searchBySession(Session *session);
    std::string getPassword();
    bool checkNicknames(std::string nick);
    size_t size();

    void joinClientChannel(std::string name, Client *client);
    void kickClientChannel(std::string name, Client *client);
    std::vector<Client *> getClientsChannel(std::string name); 

    ~Book();
	
};
 
#endif