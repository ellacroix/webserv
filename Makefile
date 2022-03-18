SRC =	server/Client.cpp \
		server/Request.cpp \
		server/Server.cpp \
		server/main.cpp \

SRC_REQUEST_TESTER =	server/Request.cpp \
						server/tester_request.cpp \

NAME = webserv
NAME_REQUEST_TESTER = request_tester
COMPILER = c++

FLAGS = -Wall -Wextra -Werror

OBJ=$(SRC:.cpp=.o)
OBJ_REQUEST_TESTER=$(SRC_REQUEST_TESTER:.cpp=.o)

.cpp.o:
	${COMPILER} ${FLAGS} -c $< -o ${<:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJ)
	@$(COMPILER) $(FLAGS) -o $(NAME) $(OBJ)

request: $(OBJ_REQUEST_TESTER)
	@$(COMPILER) $(FLAGS) -o $(NAME_REQUEST_TESTER) $(OBJ_REQUEST_TESTER)

clean:
	@rm -rf $(OBJ) $(OBJ_REQUEST_TESTER)

fclean: clean
	@rm -rf $(NAME) $(NAME_REQUEST_TESTER)

re: fclean all

leak: $(NAME) 
	valgrind --suppressions=valgrind_readline_leaks_ignore.txt --leak-check=full --show-leak-kinds=all --track-origins=yes --child-silent-after-fork=yes --log-file=valgrind-out.txt ./minishell

leaks: $(NAME)
	valgrind --suppressions=valgrind_readline_leaks_ignore.txt --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt ./minishell

.PHONY: all clean fclean re