#ifndef HEADER_HPP
#define HEADER_HPP

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <map>

#include <unistd.h>

#include <poll.h>
#include "Channel.hpp"
#include "Book.hpp"
#include <string>
#include <vector>

static const char *EXIT_MSG = "*** someone leave us ***\n";

#endif
