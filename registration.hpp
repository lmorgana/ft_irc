
#include "header.hpp"
#include "session.hpp"
#include "Client.hpp"
#include "Book.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <iterator>

const std::string SERVER = "blueCat";

// class Book
// {
// public:

// 	Client* getClient(Session *current) {}
// 	std::string	getPassword() {}
// 	bool	checkNicknames(std::string nick) {}
// };

struct returnRes
{
	std::string				msg;
	std::vector<Session*>	users;

};

// std::map<std::string, std::string> errorMsg(Book book, std::string str)
// {
// 	std::map<std::string, std::string>	error;

// 	error["451"] = ":You have not registered";
// 	error["461"] = str + " :Not enough parameters";
// 	error["462"] = ":You may not reregister";

// 	return error;
// }

std::string	toCString(std::string str)
{
	if (!str.empty())
		str += "\r\n";
	// else
	// 	str += ' ';
	return str;
}

int	l_len(std::string s, char c, int i)
{
	int	len;

	len = 0;
	while (s[i + len] != c && s[i + len])
		len++;
	return len;
}

std::vector<std::string>	ft_split(std::string s, char c)
{
	std::vector<std::string>	res;
	std::string					tmp;

	int i = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (!s[i])
			break ;
		for (int k = 0; k < l_len(s, c, i); k++)
			tmp += s[i + k];
		res.push_back(tmp);
		tmp.clear();
		while (s[i] != c && s[i])
			i++;
	}
	return res;
}

struct returnRes*	checkData(Session* current, char* buf, Book* book, struct returnRes* res)
{
	std::string str = buf;
	str = str.substr(0, str.length() - 1);
	Client*	curClient = book->getClient(current);
	std::vector<std::string> words = ft_split(str, ' ');
	
	if (curClient->getPass() && (curClient->getUser().empty() || curClient->getNick().empty()))
	{
		if (words[0] == "PASS")
			res->msg = toCString("462 :You may not reregister");
		else if (words[0] == "NICK")
		{
			// words = ft_split(str, ' ');
			if (words.size() == 1)
				res->msg = toCString("431 :No nickname given");
			else
			{
				if (!book->checkNicknames(words[1]))
					curClient->setNick(words[1]);
				else
					res->msg = toCString("433 " + words[1] + " :Nickname is already in use");
			}
		}
		else if (words[0] == "USER" && curClient->getUser().empty())
		{
			if (!curClient->getUser().empty())
				res->msg = toCString("462 :You may not reregister");
			else
			{
				// words = ft_split(str, ' ');
				if (words.size() < 5)
					res->msg = toCString("461 USER :Not enough parameters");
				else
				{
					curClient->setUser(words[1]);
					if (words[4][0] == ':')
						curClient->setRealName(words[4].substr(1, words[4].size() - 1));
				}
			}
		}
		else
			res->msg = toCString("451 :" + SERVER + " :You have not registered");
		if (!curClient->getUser().empty() && !curClient->getNick().empty())
		{
			res->msg += "375 :- " + SERVER + " Message of the day - \n";
			res->msg += "372 :- Welcome to our server!\n";
			res->msg += "376 :End of /MOTD command";
			res->msg = toCString(res->msg);
		}
	}
	else
	{
		if (words[0] == "PASS")
		{
			if (words.size() == 1)
				res->msg = toCString("461 PASS :Not enough parameters");
			else
			{
				if (words[1] == book->getPassword())
					curClient->setPass();
			}
		}
		else
			res->msg = toCString("451 :" + SERVER + " :You have not registered");
	}

	res->users.push_back(current);

	std::cout << "Pass: " << curClient->getPass() << std::endl;
	std::cout << "Nick: " << curClient->getNick() << std::endl;
	std::cout << "User: " << curClient->getUser() << std::endl;
	std::cout << "Name: " << curClient->getRealName() << std::endl;

	return res;
}