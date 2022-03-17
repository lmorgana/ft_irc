cmake_minimum_required(VERSION 3.20.2)
project(ft_irc)

add_compile_options(-Wall -Wextra -Werror -std=c++98)

set(CMAKE_CXX_STANDARD 14)

add_executable(webserv
        main.cpp
        sockets.cpp
        sockets.hpp
        server.cpp
        server.hpp
        session.cpp
        session.hpp
        Book.cpp
        Book.hpp
        Client.cpp
        Client.hpp
        registration.hpp
        Channel.cpp
        Channel.hpp)
