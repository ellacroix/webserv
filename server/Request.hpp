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

class Request{

private:

protected:

public:
	std::string raw_request;
	int error;
	bool Chunked;
	int ContentLenght;
	std::string METHOD;
	std::string headers;


	Request() {};
	~Request() {};

	Request(char *raw);

	int			parser();

};

/* class Response{
	isReponseChunked();

	int	GET();
	int POST();
	int	DELETE();
	int CGI();

};
 */