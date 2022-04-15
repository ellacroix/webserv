#ifndef WEBSERV_HPP
# define WEBSERV_HPP

//	GLIBC
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <ctime>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>

//	CPP
#include <vector>
#include <map>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <deque>
#include <algorithm>

//	CUSTOM HEADERS
/*
#include "Location.hpp"
#include "VirtualServer.hpp"
#include "Client.hpp"
#include "Port.hpp"
#include "ConfigParser.hpp"
*/

# define SUCCESS 0
# define FAILURE 1
# define RECV_BUFFER_SIZE 5000
# define THREADS 2
# define TIMEOUT 50
# define K 1000
# define M 1000000
# define SERVER_MAX_BODY_SIZE 10 * M
# define SERVER_MAX_HEADERS_SIZE 1 * M
# define MAX_CONNECTIONS 150

# define RED "\033[31m"
# define RESET "\033[0m"


class	ConfigParser ;

class	Port ;
typedef	std::list<Port*>::iterator						t_portListIt;

class	Client ;
typedef	std::map<int, Client*>::iterator				t_clientMapIt;

class	VirtualServer ;
typedef std::list<VirtualServer*>::iterator				t_VS_listIt;
typedef std::list<VirtualServer*>::const_iterator		t_VS_listCIt;

class	Location;
typedef	std::map<std::string, Location*>::iterator		t_locMapIt;

typedef struct	s_thread_info
{
	std::deque<Client*> *queue;
	pthread_mutex_t		queue_mutex;

	pthread_cond_t		condition_var;

	pthread_mutex_t		epoll_fd_mutex;
	int					*epoll_fd;
	struct epoll_event	event;

	unsigned long		start_time;

	int		start(int *epoll_fd, pthread_t *thread_pool);
}				t_thread_info;


//	ThreadsPoolRoutines.cpp
void	*threadLoop(void* arg);
void	threadRecvRoutine(Client *client, t_thread_info *thread_info);
void	threadSendRoutine(Client *client, t_thread_info *thread_info);
void	isRequestComplete(Client *client);
bool	isChunkComplete(Client *client);
bool	isBodyComplete(Client *client, size_t length);
void	createAndConstructResponse(Client *client);
void	monitorForReading(Client *client, t_thread_info *thread_info);
void	monitorForWriting(Client *client, t_thread_info *thread_info);

//	ThreadPool.cpp

//	parsingUtils.cpp
bool			isNumber(std::string s);
bool			isValidPrefix(std::string *s);
bool			isValidPort(std::string s);
bool			isValidDomainName(std::string s);
bool			isValidClientMaxBodySize(std::string & s);
bool			isValidHttpErrorCode(std::string s);
bool			isValidAbstolutePathToFile(std::string & s);
bool			isValidErrorPage(std::vector<std::string> & v);
bool			isValidAutoindex(std::string s);
bool			areValidIndexes(std::vector<std::string> & v);
bool			isValidIndex(std::string s);
bool			isSupportedHttpRedirCode(std::string s);
bool			isValidReturn(std::vector<std::string> & v);
bool			isSupportedHttpMethod(std::string s);
bool			isValidLimitExcept(std::vector<std::string> v);
bool			isValidReqUri(std::string const & s);
bool			areValidDomainNames(std::vector<std::string> & v);
std::string		numberToString(int n);
std::string		findUriExtension(std::string uri);
void	logger(std::string message);


//	engineUtils.cpp
int		startAllPorts(ConfigParser & config, struct epoll_event,
		int epoll_fd);
int		acceptIncomingConnections(Port *current_port, struct epoll_event &event,
		int epoll_fd, t_thread_info *thread_info, int *current_connections);
void    recvClientsRequest(Port *current_port, t_thread_info *thread_info,
		t_clientMapIt it_c, int *current_connections);
void	sendClientResponse(t_thread_info *thread_info,
		t_clientMapIt it_c);
int		disconnectTimeout408(std::list<Port*> ports_list, t_thread_info *thread_info, int *current_connections);
void	cleanShutDown(pthread_t *thread_pool, t_thread_info *thread_info);

//	fileUtils.cpp
bool pathExists(std::string path);
bool isDirectory(std::string path);
bool isFile(std::string path);
bool canRead(std::string path);
bool canWrite(std::string path);
bool canExecute(std::string path);

#endif
