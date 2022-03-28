#include "Request.hpp"

Request::Request(std::string raw)
{
	this->raw_request = raw;
}

int	Request::parser()
{
	printf("In the parser, working on:\n%s\n", raw_request.c_str());
	
	return 0;
}