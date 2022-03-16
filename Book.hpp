#ifndef BOOK_HPP
#define BOOK_HPP

#include "header.hpp"
#include "Client.hpp"

class Book{
	
	private:

    std::vector<Client *> clients;
    std::string _password;
    public:

    Book(char *password);
    Client *getClient(Session *session);
    bool searchBySession(Session *session);
    std::string getPassword();
    bool checkNicknames(std::string nick);
    size_t size();
    ~Book();
	
};
 
#endif