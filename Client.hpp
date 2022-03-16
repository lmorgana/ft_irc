#ifndef Client_HPP
#define Client_HPP

#include <iostream>
#include <string>
#include "session.hpp"

 class Client
 {
    private:

    std::string _nick;
    std::string _user;
    std::string _realName;
    Session     *_session;
    bool        _pass;

    public:

    Client(Session *session, std::string nick = "default", std::string user= "default", std ::string realName = "default");

    void setNick(std::string nick);
    void setUser(std::string User);
    void setRealName(std::string realName);
    void setPass();

    bool getPass() const {return _pass;}
    std::string getNick() const {return _nick;}
    std::string getUser() const {return _user;}
    std::string getRealName() const {return _realName;}
    Session *getSession() const {return _session;}

 };

 #endif