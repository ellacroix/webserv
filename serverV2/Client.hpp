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

class Client{

private:

protected:

public:
	int					stream_socket;
	bool				connected;

	//Request				*request = NULL;
	//Response			*response = NULL;

	Client() {};
	Client(int fd);
	~Client() {};

};
