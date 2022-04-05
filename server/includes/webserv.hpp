#ifndef WEBSERV_HPP
# define WEBSERV_HPP

//	GLIBC
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/epoll.h>

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
# define RECV_BUFFER_SIZE 256
# define THREADS 2
# define TIMEOUT 100

class	ConfigParser ;
class	Port ;
typedef	std::list<Port*>::iterator			t_portListIt;
class	Client ;
typedef	std::map<int, Client*>::iterator	t_clientMapIt;


typedef struct	s_thread_info
{
	std::deque<Client*> *queue;
	pthread_mutex_t		queue_mutex;

	pthread_cond_t		condition_var;

	pthread_mutex_t		epoll_fd_mutex;
	int					*epoll_fd;
	struct epoll_event	event;

	int		start(int *epoll_fd, pthread_t *thread_pool);

}				t_thread_info;


//	ThreadsPoolRoutines.cpp
void	*thread_loop(void* arg);
void	thread_recv_routine(Client *client, t_thread_info *thread_info);
void	thread_send_routine(Client *client, t_thread_info *thread_info);
//	ThreadPool.cpp

//	utils.cpp
bool			isNumber(std::string s);
bool			isValidPrefix(std::string *s);
bool			isValidPort(std::string s);
bool			isValidDomainName(std::string s);
bool			isValidClientMaxBodySize(std::string & s);
bool			isValidHttpErrorCode(std::string s);
bool			isValidFile(std::string & s);
bool			isValidErrorPage(std::vector<std::string> & v);
bool			isValidAutoindex(std::string s);
//bool			isValidIndex(std::vector<std::string> v);
bool			isValidIndex(std::string s);
bool			isSupportedHttpRedirCode(std::string s);
bool			isValidReturn(std::vector<std::string> & v);
bool			isSupportedHttpMethod(std::string s);
bool			isValidLimitExcept(std::vector<std::string> v);
bool			isValidReqUri(std::string const & s);

//	engineUtils.cpp
int		startAllPorts(ConfigParser & config, struct epoll_event,
		int epoll_fd);
int		acceptIncomingConnections(Port *current_port, struct epoll_event &event,
		int epoll_fd, t_thread_info *thread_info);
void    recvClientsRequest(Port *current_port, t_thread_info *thread_info,
		t_clientMapIt it_c);
int		DisconnectTimeout408(std::list<Port*> PortsList);




#endif
