NAME			= ft_irc
SRCS			= main.cpp sockets.cpp  server.cpp session.cpp
NAME            = ft_irc
SRCS            = main.cpp sockets.cpp  server.cpp session.cpp Book.cpp Client.cpp Channel.cpp
OBJS            = $(patsubst %.cpp, %.o, $(SRCS))
HEADERS         = header.hpp sockets.hpp server.hpp session.hpp Book.hpp Client.hpp registration.hpp Channel.hpp
CC              = clang++
RM              = rm -f
FLAGS           = -Wall -Wextra -Werror -std=c++98
%.o: %.cpp $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@
all:            $(NAME)
$(NAME):        $(HEADERS) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)
clean:
	$(RM) $(OBJS)
fclean:         clean
	$(RM) $(NAME)
re:             fclean $(NAME)
.PHONY:         all clean fclean re