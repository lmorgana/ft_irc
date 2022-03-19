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
    Session * getSession(std::string nick);
    Session * searchSession(std::string nick);
    std::vector<Session *> getAllSession();
    bool searchBySession(Session *session);
    std::string getPassword();
    bool checkNicknames(std::string nick);
    size_t size();
    bool searchChannel(std::string name);
    bool kickClient(Session *session);
    bool checkNickInChannels(std::string nick, std::vector<std::string> name_channel); 

    void joinClientChannel(std::string name, Client *client);
    bool kickClientChannel(std::string name, std::string nick);
    std::vector<Client *> * getClientsChannel(std::string name);
    bool checkHostChanel(std::string name, Client *client);
    bool checkChannel(std::string name);
    std::vector<std::string> getNickChanel(std::string name);



    ~Book();
	
};
 
#endif