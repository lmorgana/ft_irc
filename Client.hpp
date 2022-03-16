#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>

class Client
{
	std::string	_nick;
	std::string	_user;
	std::string _realName;

	bool		_pass;

public:

	Client();
    Client(std::string nick, std::string user, std ::string realName);
    std::string getNick() const {return _nick;}
    std::string getUser() const {return _user;}
    std::string getRealName() const {return _realName;}
	
	// void	setNick(mainClass irc);
	bool	getPass() {}
	void	setPass() {}

	void	setNick(std::string nick) {}
	void	setUser(std::string user) {}
	void	setRealName(std::string user) {}
};

#endif
