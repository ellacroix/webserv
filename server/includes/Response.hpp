#ifndef RESPONSE_HPP
# define RESPONSE_HPP
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
#include "Request.hpp"
#include "VirtualServer.hpp"

class Client ;
class Request ;
class VirtualServer ;

class Response {

private:

protected:

public:
	std::string		raw_response;
	std::string		body;

	int				status_code;

	Client			*client;
	Request			*request;
	VirtualServer	*virtual_server;
	Location		*location;


	Response() {};
	~Response() {};
	Response(Client *parent_client);

	int		ConstructResponse();
	void	constructError();
	std::string	getErrorMessage(int code);

	VirtualServer	*findVirtualServer(std::string & s);

};

#endif
