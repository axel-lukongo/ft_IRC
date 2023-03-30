NAME = ircserv
CXX = c++
RM = rm -f
CXXFLAGS = -Wall -Werror -Wextra -g -std=c++98
CPPFLAGS = -MMD

SRCS = srcs/main.cpp srcs/server.cpp srcs/commands.cpp srcs/utils.cpp

OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	clear
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all re clean fclean test