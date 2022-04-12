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
					ResponseConstruction.cpp \
					engineUtils.cpp \
					fileUtils.cpp \
					extensions.cpp

HEADERS			=	includes/Client.hpp \
					includes/Port.hpp \
					includes/VirtualServer.hpp \
					includes/ConfigParser.hpp \
					includes/webserv.hpp \
					includes/Location.hpp \
					includes/Request.hpp \
					includes/Response.hpp

INC_FLD			=	includes/

OBJS			=	${addprefix srcs/,${SRCS:.cpp=.o}}


CXX				=	c++

CXXFLAGS		= -pthread -Wall -Wextra -Werror -std=c++98
#CXXFLAGS		= -pthread -Wall -Wextra -Werror

SANITIZE_FLAGS	= -pthread -fsanitize=thread -Wall -Wextra -Werror -std=c++98


all			:	${NAME}

${NAME}		:	${OBJS}
			${CXX} ${CXXFLAGS} -I ${INC_FLD} ${OBJS} -o ${NAME}


%.o			:	%.cpp ${HEADERS}
			${CXX} ${CXXFLAGS} -I ${INC_FLD} -c $< -o ${<:.cpp=.o}

clean		:
			rm -rf ${OBJS}

fclean		:	clean
			rm -rf ${NAME}

re			:	fclean all

run			:	${NAME}
			./webserv confs/nginx.conf

sanitize	:	${OBJS}
			${CXX} ${SANITIZE_FLAGS} ${OBJS} -o ${NAME}
			./webserv confs/nginx.conf

valgrind	:	$(NAME)
			valgrind --suppressions=valgrind_ignore.txt ./webserv confs/nginx.conf

leaks		:	$(NAME)
			valgrind --suppressions=valgrind_ignore.txt \
			--leak-check=full --show-leak-kinds=all --track-origins=yes \
			--log-file=valgrind-out.txt \
			./webserv confs/nginx.conf

.PHONY		:	all re clean fclean
