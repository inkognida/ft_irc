SRCS = ./srcs/*.cpp

CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

HEADERS = ./headers/*.hpp

NAME = ircserv


all: $(NAME)

$(NAME): $(SRCS) $(HEADERS)
			$(CPP) $(CPPFLAGS) -o $(NAME) $(SRCS)

fclean: rm -f $(NAME)

clean: rm fclean

re: fclean all


.PHONY: re clean fclean all
