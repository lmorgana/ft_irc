cmake_minimum_required(VERSION 3.20.2)
project(webserv)

set(CMAKE_CXX_STANDARD 14)

add_executable(webserv
        main.cpp
        sockets.cpp
        sockets.hpp
        server.cpp
        server.hpp
        session.cpp
        session.hpp)
