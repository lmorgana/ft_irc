#ifndef BOOK_HPP
#define BOOK_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Client.hpp"
#include "session.hpp"

class Book{
	
	private:

    std::vector<Client *> clients;

    public:

    Client *getClient(Session *session);
    bool searchBySession(Session *session);
    size_t size();
    ~Book();
	
};
 
#endif