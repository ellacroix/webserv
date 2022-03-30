#include "Request.hpp"

Request::Request(std::string raw)
{
	this->raw_request = raw;
	stream_request.str(raw_request);
	parsing_step = FIRST_LINE;
}

int	Request::parser()
{
	printf("In the parser, working on:\n%s\n", raw_request.c_str());

	int ret;
	
	if (parsing_step == FIRST_LINE)
		ret = parseFirstLine();
	if (parsing_step == HEADERS)
		ret = parseHeaders();
/* 	if (parsing_step == PREBODY)
		ret = prebody();
	if (parsing_step == BODY)
		ret = body();
	if (parsing_step == CHUNK)
		ret = chunk(); */
	
	if (parsing_step == COMPLETE && ret == 1)
		return ret;
	
	if (ret > 1)
		return ret;

	return 0;
}

int Request::parseFirstLine()
{
	std::string tmp;

	std::getline(stream_request, tmp, ' ');
	if (tmp != "GET" || tmp != "POST" || tmp != "DELETE")
		return 501;
	method = tmp;

	std::getline(stream_request, tmp, ' ');
	if (tmp[0] != '/')
		return 400;
	URI = tmp;

	std::getline(stream_request, tmp);
	if (tmp != "HTTP/1.1")
		return 505;

	parsing_step = HEADERS;
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
