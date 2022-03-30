NAME		=	webserv

SRCS		=	Client.cpp \
				main.cpp \
				Port.cpp \
				Location.cpp \
				VirtualServer.cpp \
				ConfigParser.cpp \
				ConfigParserFile.cpp \
				ConfigParserDirArgs.cpp \
				utils.cpp \
				Request.cpp \

OBJS		=	${SRCS:.cpp=.o}

CXX			=	c++

CXXFLAGS	= -pthread -Wall -Wextra -Werror -std=c++98

all			:	${NAME}

${NAME}		:	${OBJS}
			${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

%.o			:	%.cpp Client.hpp Port.hpp VirtualServer.hpp ConfigParser.hpp \
	webserv.hpp \
	Location.hpp
			${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

clean		:
			rm -rf ${OBJS}

fclean		:	clean
			rm -rf ${NAME}

re			:	fclean all

.PHONY		:	all re clean fclean
