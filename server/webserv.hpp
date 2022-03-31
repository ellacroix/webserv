#ifndef WEBSERV_HPP
# define WEBSERV_HPP

//	GLIBC
#include <stdio.h>
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

class Client ;

typedef struct	s_thread_info
{
	std::deque<Client*> *queue;
	pthread_mutex_t		queue_mutex;

	pthread_cond_t		condition_var;

	pthread_mutex_t		epoll_fd_mutex;
	int					*epoll_fd;
	struct epoll_event	event;

}				t_thread_info;


// ThreadsPool.cpp
void	*thread_loop(void* arg);
void	thread_recv_routine(Client *client, t_thread_info *thread_info);
void	thread_send_routine(Client *client, t_thread_info *thread_info);


//	utils.cpp
bool			isNumber(std::string s);
bool			isValidPrefix(std::string *s);
bool			isValidPort(std::string s);
bool			isValidDomainName(std::string s);
bool			isValidClientMaxBodySize(std::string & s);
bool			isValidHttpErrorCode(std::string s);
bool			isValidUri(std::string & s);
bool			isValidErrorPage(std::vector<std::string> & v);
bool			isValidAutoindex(std::string s);
//bool			isValidIndex(std::vector<std::string> v);
bool			isValidIndex(std::string s);
bool			isSupportedHttpRedirCode(std::string s);
bool			isValidReturn(std::vector<std::string> & v);
bool			isSupportedHttpMethod(std::string s);
bool			isValidLimitExcept(std::vector<std::string> v);

#endif
