
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

std::vector<std::string>	ft_split_mod(std::string s, char c)
{
	std::vector<std::string>	res;
	std::string					tmp;

	int count = 0;
	int i = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (res.size() >= 2)
			break;
		if (!s[i])
			break ;
		for (int k = 0; k < l_len(s, c, i); k++)
			tmp += s[i + k];
		res.push_back(tmp);
		count++;
		tmp.clear();
		while (s[i] != c && s[i])
			i++;
	}
	for (int j = i; s[j]; j++)
		tmp += s[j];
	res.push_back(tmp);
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
	if (!words.empty() && words[0] == "PASS")
	{
		if (words.size() == 1)
			res.msg = resultString("461 PASS :Not enough parameters");
		else
		{
			std::string pas = words[1];
			if (pas == book->getPassword()
				|| ((pas[0] == ':')
				&& (pas.substr(1, pas.size() - 1)) == book->getPassword()))
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
											std::string str,
											std::vector<struct returnRes>* result)
{
	std::string					privMsg;
	std::vector<std::string>	recipients;
	std::vector<std::string>	nicks;
	std::vector<std::string>	channels;
	std::vector<std::string>	words;

	words = ft_split_mod(str, ' ');
	if (words.size() < 3)
		res.msg = resultString("412 :No text to send");
	else
	{
		recipients = ft_split(words[1], ',');
		for (size_t i = 0; i < recipients.size(); i++)
		{
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
			if (recipients[i][0] == '#' || recipients[i][0] == '&')
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
				res.msg = ":" + curClient->getNick() + "!" + curClient->getUser();
				res.msg += "@127.0.0.1 " + words[0] + " " + recipients[i] + " ";
				res.msg += resultString(words[2]);
				if (recipients[i][0] == '#' || recipients[i][0] == '&')
				{
					std::vector<std::string> checkChannel;
					checkChannel.push_back(recipients[i]);
					if (book->checkNickInChannels(curClient->getNick(), checkChannel))
					{
						std::vector<Client*> *clientRes = book->getClientsChannel(recipients[i]);
						for (size_t j = 0; j < clientRes->size(); j++)
						{
							res.users = (*clientRes)[j]->getSession();
							result->push_back(res);
						}
					}
					else
						res.msg = resultString("404 " + recipients[i] + " :Cannot send to channel");
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

std::vector<struct returnRes>*	joinMethod(Book* book,
											struct returnRes res,
											Client* curClient,
											std::vector<std::string> words,
											std::vector<struct returnRes>* result)
{
	std::vector<std::string> channels;

	if (words.size() == 1)
		res.msg = resultString("461 " + words[0] + " :Not enough parameters");
	else
	{
		channels = ft_split(words[1], ',');
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i][0] == '#' || channels[i][0] == '&')
			{
				book->joinClientChannel(channels[i], curClient);
				std::string	msgForAll;

				res.msg = "331 " + curClient->getNick() + " ";
				res.msg += channels[i] + " :No topic is set";

				res.msg += "\n353 " + curClient->getNick() + " = ";
				res.msg += channels[i] + " :";
				for (size_t j = 0; j < book->getNickChanel(channels[i]).size(); j++)
				{
					if (j)
						res.msg += " ";
					res.msg += "@" + book->getNickChanel(channels[i])[j];
				}

				res.msg += "\n366 " + curClient->getNick() + " ";
				res.msg += channels[i] + " :End of /NAMES list";

				res.msg = resultString(res.msg);
				res.users = curClient->getSession();
				result->push_back(res);
				
				std::vector<Client *>	otherClients = *(book->getClientsChannel(channels[i]));
				for (size_t k = 0; k < otherClients.size() - 1; k++)
				{
					res.msg = "127.0.0.1 " + curClient->getNick() + " JOIN " + channels[i];
					res.msg = resultString(msgForAll);
					res.users = otherClients[k]->getSession();
					result->push_back(res);
				}
			}
			else
			{
				res.msg = resultString("403 " + channels[i] + " :No such channel");
				break ;
			}
		}
	}
	if (result->empty())
	{
		res.users = curClient->getSession();
		result->push_back(res);
	}
	return result;
}

std::vector<struct returnRes>*	kickMethod(Book* book, struct returnRes res,
										Client* curClient,
										std::vector<std::string> words,
										std::vector<struct returnRes>* result)
{
	if (words.size() < 3)
		res.msg = resultString("461 " + words[0] + " :Not enough parameters");
	else
	{
		if (book->checkChannel(words[1]))
		{
			if (book->checkHostChanel(words[1], curClient))
			{
				if (book->kickClientChannel(words[1], words[2]))
				{
					res.msg = "You were kicked from " + words[1] + "\n";
					res.users = book->getSession(words[2]);
					result->push_back(res);
				}
				else
					res.msg = resultString("441 " + words[2] + " " + words[1]\
											 + " :They aren't on that channel");
			}
			else
				res.msg = resultString("482 " + words[1] + " :You're not channel operator");
		}
		else
			res.msg = resultString("403 " + words[1] + " :No such channel");
	}
	if (result->empty())
	{
		res.users = curClient->getSession();
		result->push_back(res);
	}
	return result;
}

std::vector<struct returnRes>*	botMethod(Book* book, struct returnRes res,
										std::vector<struct returnRes>* result)
{
	std::vector<Client *> clients = book->getAllClients();
	for (size_t i = 0; i < clients.size(); i++)
	{
		res.msg = ":" + clients[i]->getNick() + "!" + clients[i]->getUser();
		res.msg += "@127.0.0.1 PRIVMSG " + clients[i]->getNick() + " :";
		res.msg += "AHAHA BOT hacked you!!!\n";
		res.msg += "       _  __________=__\n";
		res.msg += "        \\@([____]_____()\n";
		res.msg += "       _/\\|-[____]\n";
		res.msg += "      /     /(( )\n";
		res.msg += "     /____|'----'\n";
		res.msg += "     \\____/\n";
		res.msg = resultString(res.msg);
		res.users = clients[i]->getSession();
		result->push_back(res);
	}
	return result;
}

std::vector<struct returnRes>*	checkData(Session* current, char* buf,
										Book* book, std::vector<struct returnRes>* result)
{
	std::string str = buf;
	if (str.length() >= 2 && str.substr(str.length() - 2, 2) == "\r\n")
		str = str.substr(0, str.length() - 2);
	else
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
		if (words.empty())
			res.msg = resultString("421 :Unknown command");
		else if (words[0] == "NICK")
			res = nickMethod(book, res, curClient, words);
		else if (words[0] == "PRIVMSG" || words[0] == "NOTICE")
			result = privMsgMethod(book, res, curClient, str, result);
		else if (words[0] == "JOIN")
			result = joinMethod(book, res, curClient, words, result);
		else if (words[0] == "KICK")
			result = kickMethod(book, res, curClient, words, result);
		else if (words[0] == "BOT")
			result = botMethod(book, res, result);
		else
			res.msg = resultString("421 " + words[0] + " :Unknown command");
	}
	else
		res = passMethod(book, res, curClient, words);
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