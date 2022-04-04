#include "Request.hpp"

Request::Request(std::string raw, Client *parent_client)
{
	client = parent_client;
	this->raw_request = raw;
}

int	Request::parser()
{
	printf("In the parser, working on:\n%s\n", raw_request.c_str());
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
