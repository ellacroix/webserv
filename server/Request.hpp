#ifndef REQUEST_HPP
# define REQUEST_HPP
/*
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
*/


#include "webserv.hpp"

#include "Client.hpp"
#include "VirtualServer.hpp"

class Client ;
class VirtualServer ;

class Request {

private:

protected:

public:
	std::string			raw_request;
	std::istringstream	stream_request;

	int				status_code;
	bool Chunked;
	int ContentLength;
	std::string method;
	std::string URI;
	std::string headers;

	Client			*client;
	VirtualServer	*virtual_server;


	Request() {};
	~Request() {};
	Request(std::string raw);

	int			parser();
	int			parseFirstLine();
	int			parseHeaders();

	enum Step {
		FIRST_LINE,
		HEADERS,
		PREBODY,
		BODY,
		CHUNK,
		COMPLETE,
		ERROR
	};
	Step parsing_step;
};

#endif
