#ifndef RESPONSE_HPP
# define RESPONSE_HPP

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

	std::string		path;
	//	RESPONSE DATA
	std::string		raw_response;
	std::string		body;
	int				status_code;
	std::ifstream	request_file;
	size_t			file_len;
	bool			chunked;

	//	HEADERS
	std::string		location_header;

	//	CONSTRUCTOR / DESTRUCTOR
	Response() {};
	~Response() {};
	Response(Client *parent_client);

	//	RESPONSE PROCESSING
	int			ConstructResponse();
	void		constructError();
	void		constructSuccess(void);
	std::string	getErrorMessage(int code);

	//	FILE - ResponseProcessing.cpp
	bool			findIndex(void);
	void		constructAutoIndex();
	std::string	getErrorMessage(int code);

	//	FILE - ResponsePorcessing.cpp
	VirtualServer *	findVirtualServer(std::string & s);
	Location *		findLocation(std::string & URI);

};

#endif
