#pragma once

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

class Client{

private:

protected:

public:
	int					stream_socket;
	struct sockaddr_in	client_address;
	bool				connected = true;
	std::string			buffer;

	Request				*request = NULL;

	void				createRequest(char *raw);

	Client() {};
	~Client();

};