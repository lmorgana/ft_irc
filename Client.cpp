#include "Client.hpp"

Client::Client(Session *session)
{
    this->_pass = false;
    this->_authorized = false;
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

 void Client::setAuthorized()
 {
     _authorized = true;
 }
 