NAME		=	webserv

SRCS		=	main.cpp \
				Client.cpp \
				Port.cpp \
				Virtual_Server.cpp

OBJS		=	${SRCS:.cpp=.o}

CXX			=	c++

CXXFLAGS	= -pthread -Wall -Werror -Wextra -std=c++98

all			:	${NAME}

${NAME}		:	${OBJS}
			${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

%.o			:	%.cpp Client.hpp Port.hpp Virtual_Server.hpp 
			${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

clean		:
			rm -rf ${OBJS}

fclean		:	clean
			rm -rf ${NAME}

re			:	fclean all

.PHONY		:	all re clean fclean
