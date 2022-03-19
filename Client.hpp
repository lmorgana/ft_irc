#ifndef Client_HPP
#define Client_HPP

# include "header.hpp"

 class Session;

 class Client
 {
    private:

    std::string _nick;
    std::string _user;
    std::string _realName;
    Session     *_session;
    bool        _pass;
    bool        _authorized;

    public:

    Client(Session *session);

    void setNick(std::string nick);
    void setUser(std::string User);
    void setRealName(std::string realName);
    void setPass();
    void setAuthorized();

    bool getAuthorized(){return _authorized;}
    bool getPass() const {return _pass;}
    std::string getNick() const {return _nick;}
    std::string getUser() const {return _user;}
    std::string getRealName() const {return _realName;}
    Session *getSession() const {return _session;}

 };

 #endif
 