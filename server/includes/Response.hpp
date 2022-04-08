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
	//	DATA RESPONSE DEPENDS ON
	Client			*client;
	Request			*request;
	VirtualServer	*virtual_server;
	Location		*location;

	//	RESPONSE DATA
	std::string		raw_response;
	std::string		body;
	int				status_code;
	std::string		location_header;
	std::string		path;

	//	CONSTRUCTOR / DESTRUCTOR
	Response() {};
	~Response() {};
	Response(Client *parent_client);

	//	RESPONSE PROCESSING
	int			ConstructResponse();
	void		constructError();
	std::string	getErrorMessage(int code);

	//	FILE - ResponsePorcessing.cpp
	VirtualServer *	findVirtualServer(std::string & s);
	Location *		findLocation(std::string & URI);

};

#endif
