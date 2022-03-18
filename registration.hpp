
#ifndef REGISTRATION_HPP
#define REGISTRATION_HPP

#include "header.hpp"
#include "session.hpp"
#include "Client.hpp"
#include "Book.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <ctype.h>

const std::string SERVER = "blueCat";

// class Book
// {
// public:

// 	Client* getClient(Session *current) {}
// 	std::string	getPassword() {}
// 	bool	checkNicknames(std::string nick) {}
// };

// std::map<std::string, std::string> errorMsg(Book book, std::string str)
// {
// 	std::map<std::string, std::string>	error;

// 	error["451"] = ":You have not registered";
// 	error["461"] = str + " :Not enough parameters";
// 	error["462"] = ":You may not reregister";

// 	return error;
// }

std::string	resultString(std::string str)
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

struct returnRes	nickMethod(Book* book, struct returnRes res,
								Client* curClient, std::vector<std::string> words)
{
	if (words.size() == 1)
		res.msg = resultString("431 :No nickname given");
	else
	{
		if (!book->checkNicknames(words[1]))
		{
			int flag = 0;
			for (size_t i = 0; i < words[1].size(); i++)
			{
				if (isalnum(words[1][i]) || words[1][i] == '-'
					|| (words[1][i] >= '[' && words[1][i] <= '^')
					|| words[1][i] == '{' || words[1][i] == '}')
					continue ;
				else
				{
					flag = 1;
					break ;
				}
			}
			if (flag || words[1].size() > 9)
				res.msg = resultString("432 " + words[1] + " :Erroneus nickname");
			else
				curClient->setNick(words[1]);
		}
		else
			res.msg = resultString("433 " + words[1] + " :Nickname is already in use");
	}
	return res;
}

struct returnRes	userMethod(struct returnRes res, Client* curClient,
								std::vector<std::string> words)
{
	if (!curClient->getUser().empty())
		res.msg = resultString("462 :You may not reregister");
	else
	{
		if (words.size() < 5)
			res.msg = resultString("461 USER :Not enough parameters");
		else
		{
			curClient->setUser(words[1]);
			if (words[4][0] == ':')
				curClient->setRealName(words[4].substr(1, words[4].size() - 1));
		}
	}
	return res;
}

struct returnRes	passMethod(Book* book, struct returnRes res,
								Client* curClient, std::vector<std::string> words)
{
	if (words[0] == "PASS")
	{
		if (words.size() == 1)
			res.msg = resultString("461 PASS :Not enough parameters");
		else
		{
			if (words[1] == book->getPassword())
				curClient->setPass();
		}
	}
	else
		res.msg = resultString("451 :" + SERVER + " :You have not registered");
	return res;
}

std::vector<struct returnRes>*	privMsgMethod(Book* book,
											struct returnRes res,
											Client* curClient,
											std::vector<std::string> words,
											std::vector<struct returnRes>* result)
{
	std::string					privMsg;
	std::vector<std::string>	recipients;
	std::vector<std::string>	nicks;
	std::vector<std::string>	channels;

	if (words.size() < 3)
		res.msg = resultString("412 :No text to send");
	else
	{
		recipients = ft_split(words[1], ',');
		for (size_t i = 0; i < recipients.size(); i++)
		{
			std::cout << "Rec " << recipients[i] << std::endl;
			if (!book->checkNicknames(recipients[i]) && !book->searchChannel(recipients[i]))
			{
				res.msg = resultString("401 " + recipients[i] + " :No such nick/channel");
				res.users = curClient->getSession();
				result->push_back(res);
				return result;
			}
		}
		for (size_t i = 0; i < recipients.size(); i++)
		{
			if (recipients[i][0] == '#')
				channels.push_back(recipients[i]);
			else
				nicks.push_back(recipients[i]);
		}
		if (!channels.empty() && !nicks.empty())
		{
			for (size_t i = 0; i < nicks.size(); i++)
			{
				if (book->checkNickInChannels(nicks[i], channels))
				{
					res.msg = resultString("407 " + nicks[i] + " :Duplicate recipients. No message delivered");
					res.users = curClient->getSession();
					result->push_back(res);
					return result;
				}
			}
		}
		if (words[2][0] == ':')
		{
			for (size_t i = 0; i < recipients.size(); i++)
			{
				res.msg += ":" + curClient->getNick() + "!" + curClient->getUser();
				res.msg += "@127.0.0.1 PRIVMSG " + recipients[i] + " ";
				for (size_t i = 2; i < words.size(); i++)
				{
					res.msg += words[i];
					if (i != words.size() - 1)
						res.msg += " ";
				}
				res.msg = resultString(res.msg);
				if (recipients[i][0] == '#' || recipients[i][0] == '&')
				{
					std::vector<Client*> *clientRes = book->getClientsChannel(recipients[i]);
					for (size_t j = 0; j < clientRes->size(); j++)
					{
						res.users = (*clientRes)[j]->getSession();
						result->push_back(res);
					}
				}
				else
				{
					res.users = (*book).getSession(recipients[i]);
					result->push_back(res);
				}
			}
		}
		else
			res.msg = resultString("412 :No text to send");
	}
	if (result->empty())
	{
		res.users = curClient->getSession();
		result->push_back(res);
	}
	return result;
}

struct returnRes	joinMethod(Book* book, struct returnRes res,
								Client* curClient, std::vector<std::string> words)
{
	std::vector<std::string> channels;

	if (words.size() == 1)
		res.msg = resultString("461 JOIN :Not enough parameters");
	else
	{
		channels = ft_split(words[1], ',');
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i][0] == '#' || channels[i][0] == '&')
				book->joinClientChannel(channels[i], curClient);
			// else
			// 	// TODO
		}
	}
	return res;
}

struct returnRes	kickMethod(Book* book, struct returnRes res,
								Client* curClient, std::vector<std::string> words)
{
	if (words.size() < 3)
		res.msg = resultString("461 JOIN :Not enough parameters");
	else
	{
		// if (words[1])
	}
}

std::vector<struct returnRes>*	checkData(Session* current, char* buf,
										Book* book, std::vector<struct returnRes>* result)
{
	std::string str = buf;
	str = str.substr(0, str.length() - 1);
	Client*	curClient = book->getClient(current);
	std::vector<std::string> words = ft_split(str, ' ');

	struct returnRes res;
	
	if (curClient->getPass() && (curClient->getUser().empty() || curClient->getNick().empty()))
	{
		if (words[0] == "PASS")
			res.msg = resultString("462 :You may not reregister");
		else if (words[0] == "NICK")
			res = nickMethod(book, res, curClient, words);
		else if (words[0] == "USER" && curClient->getUser().empty())
			res = userMethod(res, curClient, words);
		else
			res.msg = resultString("451 :" + SERVER + " :You have not registered");
		if (!curClient->getUser().empty() && !curClient->getNick().empty())
		{
			res.msg += "375 :- " + SERVER + " Message of the day - \n";
			res.msg += "372 :- Welcome to our server!\n";
			res.msg += "376 :End of /MOTD command";
			res.msg = resultString(res.msg);
		}
		if (!curClient->getUser().empty() && !curClient->getNick().empty())
			curClient->setAuthorized();
	}
	else if (curClient->getAuthorized())
	{
		if (words[0] == "NICK")
			res = nickMethod(book, res, curClient, words);
		else if (words[0] == "PRIVMSG" || words[0] == "NOTICE")
			result = privMsgMethod(book, res, curClient, words, result);
		else if (words[0] == "JOIN")
			res = joinMethod(book, res, curClient, words);
		else
			res.msg = resultString("421 " + words[0] + " :Unknown command");
	}
	else
		res = passMethod(book, res, curClient, words);

	//FIXME when there are several users
	// if (!res.users)
	// 	res.users = current;

	//FIXME when there are several users
	if (result->empty())
	{
		res.users = current;
		result->push_back(res);
	}

	std::cout << "Pass: " << curClient->getPass() << std::endl;
	std::cout << "Nick: " << curClient->getNick() << std::endl;
	std::cout << "User: " << curClient->getUser() << std::endl;
	std::cout << "Name: " << curClient->getRealName() << std::endl;

	return result;
}

#endif