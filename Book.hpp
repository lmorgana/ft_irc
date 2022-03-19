#ifndef BOOK_HPP
#define BOOK_HPP

# include "header.hpp"
# include "Client.hpp"
# include <vector>
# include "Channel.hpp"

class Session;
class Channel;

class Book{
	
	private:

    std::vector<Client *>   clients;
    std::vector<Channel *>  channels; 
    std::string             _password;

    public:

    Book(char *password);
    ~Book();

    Client                      *getClient(Session *session);
    Session                     *getSession(std::string nick);
    std::string                 getPassword();
    Session                     *searchSession(std::string nick);
    std::vector<Session *>      getAllSession();
    std::vector<std::string>    getNickChanel(std::string name);
    std::vector<Client *>       getAllClients();
    std::vector<Client *>       *getClientsChannel(std::string name);

    bool    searchBySession(Session *session);
    bool    checkNicknames(std::string nick);
    size_t  size();
    bool    searchChannel(std::string name);
    bool    kickClient(Session *session); 
    void    joinClientChannel(std::string name, Client *client);
    bool    kickClientChannel(std::string name, std::string nick);
    bool    checkNickInChannels(std::string nick, std::vector<std::string> name_channel);
    bool    checkHostChanel(std::string name, Client *client);
    bool    checkChannel(std::string name);
	
};
 
#endif
