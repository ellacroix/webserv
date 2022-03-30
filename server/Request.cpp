#include "Request.hpp"

Request::Request(std::string raw)
{
	this->raw_request = raw;
	stream_request.str(raw_request);
}

int	Request::parser()
{
	printf("In the parser, working on:\n%s\n", raw_request.c_str());
	
	if (parseFirstLine() != 0)
		return 1;
	if (parseHeaders() != 0)
		return 1;
	
	return 0;
}

int Request::parseFirstLine()
{
	std::string line;
	
	std::getline(stream_request, line);
	std::cout << line << std::endl;

	return 0;
}

int Request::parseHeaders()
{
	std::string line;
	
	while (std::getline(stream_request, line))
	{
		std::cout << line << std::endl;
	}

	return 0;
}
