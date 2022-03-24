NAME		=	webserv

SRCS		=	main.cpp \
				Client.cpp \
				Port.cpp \
				Virtual_Server.cpp \
				ConfigParser.cpp \
				parsing.cpp

OBJS		=	${SRCS:.cpp=.o}

CXX			=	c++

CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98

all			:	${NAME}

${NAME}		:	${OBJS}
			${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

%.o			:	%.cpp Client.hpp Port.hpp Virtual_Server.hpp ConfigParser.hpp
			${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

clean		:
			rm -rf ${OBJS}

fclean		:	clean
			rm -rf ${NAME}

re			:	fclean all

.PHONY		:	all re clean fclean
