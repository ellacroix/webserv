NAME		=	webserv

SRCS		=	Client.cpp \
				main.cpp \
				Port.cpp \
				Request.cpp \
				Virtual_Server.cpp

OBJS		=	${SRCS:.cpp=.o}

CXX			=	c++

CXXFLAGS	= -pthread -std=c++98

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
