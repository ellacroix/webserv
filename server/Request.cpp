#include "Request.hpp"

Request::Request(char *raw)
{
	this->raw_request = raw;
}

int	Request::parser()
{
	printf("In the parser\n");
	
	return 0;
}