#include "Client.hpp"

Client::Client(Session *session, std::string nick , std::string user, std ::string realName)
{
    this->_nick = nick;
    this->_user = user;
    this->_realName = realName;
    this->_pass = false;
    this->_session = session;
}

void Client::setNick(std::string nick)
{
    this->_nick = nick;
}

void Client::setUser(std::string user)
{
    this->_user = user;
}

void Client::setRealName(std::string realName)
{
    this->_realName = realName;
}

void Client::setPass()
{
    _pass = true;
}