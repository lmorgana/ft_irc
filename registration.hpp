
#include "header.hpp"
#include "session.hpp"
#include "Client.hpp"
#include "Book.hpp"

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
	const char*				msg;
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

const char*	toCString(std::string str)
{
	str += "\r\n";
	return str.c_str();
}

returnRes*	checkData(Session* current, char* buf, Book* book)
{
	std::string str = buf;
	// std::map<std::string, std::string> errorBook = errorMsg(book, str);
	// std::map<std::string, std::string>::iterator it;
	Client*	curClient = book->getClient(current);
	returnRes*	res = new returnRes;
	std::string	resStr;
	std::string	dataFromStr;
	
	//delete \r\n from the end
	str = str.substr(0, str.length() - 1);
	int i = 0;
	if (curClient->getPass())
	{
		if (str.substr(0, 4) == "PASS")
			res->msg = toCString("461 PASS :Not enough parameters");
		else if (str.substr(0, 4) == "NICK")
		{
			i = 4;
			while (str[i] == ' ')
				i++;
			if (!str[i])
				res->msg = toCString("431 :No nickname given");
			for (int j = 0; str[i] || str[i] == ' '; j++, i++)
				dataFromStr[j] = str[i];
			if (!book->checkNicknames(dataFromStr))
				curClient->setNick(dataFromStr);
			else
				res->msg = toCString("433 " + dataFromStr + " :Nickname is already in use");
		}
		else if (str.substr(0, 4) == "USER")
		{
			if (!curClient->getUser().empty())
				res->msg = toCString("462 :You may not reregister");
			else
			{
				i = 4;
				while (str[i] == ' ')
					i++;
				for (int j = 0; str[i] || str[i] == ' '; j++, i++)
					dataFromStr[j] = str[i];
				if (!str[i])
					res->msg = toCString("461 USER :Not enough parameters");
				for (int j = 0; str[i] || str[i] == ' '; j++, i++);
				if (!str[i])
					res->msg = toCString("461 USER :Not enough parameters");
				for (int j = 0; str[i] || str[i] == ' '; j++, i++);
				if (str[i] && str[i] == ':' && str[i + 1])
				{
					i++;
					curClient->setUser(dataFromStr);
					dataFromStr = "";
					for (int j = 0; str[i]; j++, i++)
						dataFromStr[j] = str[i];
					curClient->setRealName(dataFromStr);
				}
				else
					res->msg = toCString("461 USER :Not enough parameters");
			}
		}
	}
	else
	{
		if (str.substr(0, 4) == "PASS")
		{
			i = 4;
			while (str[i] == ' ')
				i++;
			if (!str[i])
				res->msg = toCString("461 PASS :Not enough parameters");
			for (int j = 0; str[i] || str[i] == ' '; j++, i++)
				dataFromStr += str[i];
			std::cout << "dataFromStr " << dataFromStr << std::endl;
			if (dataFromStr == book->getPassword())
				curClient->setPass();
		}
		else
			res->msg = toCString("451 :" + SERVER + " :You have not registered");
		// {
		// 	it = errorbook->find("451");
		// 	resStr = ":" + SERVER + it->first + "  :" + it->second + "\r\n";
		// 	res->msg = resStr.c_str();
		// }
	}
	if (!res->msg || res->msg[0] == '\0')
		res->msg = toCString(resStr);
	res->users.push_back(current);

	return res;
}