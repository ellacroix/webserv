#pragma once

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
#include <sstream>

#include "Client.hpp"
#include "Virtual_Server.hpp"

class Client ;
class Virtual_Server ;

class Request{

private:

protected:

public:
	std::string			raw_request;
	std::istringstream	stream_request;

	int status_code;
	bool Chunked;
	int ContentLength;
	std::string METHOD;
	std::string headers;

	Client			*client;
	Virtual_Server	*virtual_server;


	Request() {};
	~Request() {};
	Request(std::string raw);

	int			parser();
	int			parseFirstLine();
	int			parseHeaders();

};

/* class Response{
	isReponseChunked();

	int	GET();
	int POST();
	int	DELETE();
	int CGI();

};
 */