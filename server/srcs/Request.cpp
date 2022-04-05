#include "Request.hpp"

Request::Request(std::string headers, Client *parent_client)
{
	client = parent_client;
	this->headers = headers;
}

int	Request::parser()
{
	printf("In the parser, working on:\n%s\n", headers.c_str());
	printf("Body:\n%s\n", body.c_str());

	return 0;
}

int Request::parseFirstLine()
{
	return 0;
}

int Request::parseHeaders()
{
	return 0;
}
