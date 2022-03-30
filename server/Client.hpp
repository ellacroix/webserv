#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Port.hpp"
#include "Request.hpp"
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
#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>


class Port ;
class Request ;

class Client{

private:

protected:

public:
	Port				*parent_port;
	int					stream_socket;
	bool				connected;
	std::string			request_buffer;
	pthread_mutex_t		client_mutex;
	struct timeval		last_activity;

	bool				response_ready;

	Request				*request;
	//Response			*response;

	Client() {};
	Client(int fd, Port *port);
	~Client();

	bool	CheckCompleteRequest();
	int		CreateRequest();

};

#endif