NAME			=	webserv

SRCS			=	Client.cpp \
					main.cpp \
					Port.cpp \
					Location.cpp \
					VirtualServer.cpp \
					ConfigParser.cpp \
					ConfigParserFile.cpp \
					ConfigParserDirArgs.cpp \
					ThreadsPoolRoutines.cpp \
					ThreadsPool.cpp \
					parsingUtils.cpp \
					Request.cpp \
					RequestHeaderArgs.cpp \
					Response.cpp \
					ResponseProcessing.cpp \
					Cgi.cpp \
					cgiUtils.cpp \
					ResponseConstruction.cpp \
					engineUtils.cpp \
					fileUtils.cpp \
					extensions.cpp \

HEADERS			=	includes/Client.hpp \
					includes/Port.hpp \
					includes/VirtualServer.hpp \
					includes/ConfigParser.hpp \
					includes/webserv.hpp \
					includes/Location.hpp \
					includes/Request.hpp \
					includes/Response.hpp \

INC_FLD			=	includes/

OBJS			=	${addprefix srcs/,${SRCS:.cpp=.o}}

CXX				=	c++

CXXFLAGS		=	-pthread -Wall -Wextra -Werror -std=c++98 -g3

SANITIZE_FLAGS	=	-pthread -fsanitize=thread -Wall -Wextra -Werror -std=c++98 -g3


all			:	${NAME}

${NAME}		:	${OBJS}
			${CXX} ${CXXFLAGS} -I ${INC_FLD} ${OBJS} -o ${NAME}


%.o			:	%.cpp ${HEADERS}
			${CXX} ${CXXFLAGS} -I ${INC_FLD} -c $< -o ${<:.cpp=.o}
#			${CXX} ${SANITIZE_FLAGS} -I ${INC_FLD} -c $< -o ${<:.cpp=.o}

clean		:
			rm -rf ${OBJS}

fclean		:	clean
			rm -rf ${NAME}

re			:	fclean all

run			:	${NAME}
			clear
			./webserv confs/nginx.conf

conf		:
			sed "s|_PWD|$PWD|g" confs/template.conf > perso.conf


sanitize	:	${OBJS}
			${CXX} ${SANITIZE_FLAGS} ${OBJS} -o ${NAME}
			./webserv confs/nginx.conf

valgrind	:	$(NAME)
			valgrind --suppressions=valgrind_ignore.txt ./webserv confs/nginx.conf

ctags		:
			ctags srcs/*.cpp
			ctags includes/*.hpp


leaks		:	$(NAME)
			valgrind --suppressions=valgrind_ignore.txt \
			--child-silent-after-fork=yes \
			--leak-check=full --show-leak-kinds=all --track-origins=yes \
			--log-file=valgrind-out.txt \
			./webserv confs/nginx.conf

.PHONY		:	all re clean fclean
