#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "webserv.hpp"

#include "Client.hpp"
#include "VirtualServer.hpp"

class Client ;
class VirtualServer ;

class Request {

private:

protected:

public:

	int				status_code;
	bool Chunked;
	int ContentLength;
	std::string method;
	std::string URI;

	std::string		headers;
	std::string		body;

	Client			*client;
	VirtualServer	*virtual_server;


	Request() {};
	~Request() {};
	Request(std::string raw, Client *parent_client);

	int			parser();
	int			parseFirstLine();
	int			parseHeaders();

};

#endif
