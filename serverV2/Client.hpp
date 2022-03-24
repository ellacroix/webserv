#pragma once

//#include "Request.hpp"
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
#include <pthread.h>

class Client{

private:

protected:

public:
	int					stream_socket;
	bool				connected;
	std::string			request_buffer;
	pthread_mutex_t		client_mutex;

	bool				response;

	//Request			*request;
	//Response			*response;

	Client() {};
	Client(int fd);
	~Client() {};

};
