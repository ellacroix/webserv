#include "webserv.hpp"
#include "ConfigParser.hpp"

int		ConfigParser::validateServerArgs(void)
{
	/*
	if (this->_line.size() == 1)
	{
		this->_context = SWITCHING_TO_SERVER;
		return (true);
	}
	else*/
	if (this->_line.size() == 2
		&& this->_line[1] == ConfigParser::_directives[OPENING_BRACKET])
		return (true);
	return (ARG_ERROR);
}

/*
int ConfigParser::validateListenArgs(void)
{
	//	3 patterns
	//	address:port
	//	address (port = 80)
	//	port (address = 0.0.0.0)
}

int ConfigParser::validateServerNameArgs(void)
{
	// 1* (alphanum_STR 1* ['.' alphanumstr] '.' domainname LWS)
}

int ConfigParser::validateClientMaxBodySizeArgs(void)
{
	//	1* DIGITS 1(k | m)
}

int ConfigParser::validateLocationArgs(void)
{
	// location prefix {
	// location prefix
}

int ConfigParser::validateRootArgs(void)
{
	//	root prefix
}

int ConfigParser::validateErrorPageArgs(void)
{
	//	error_page HTTP_CODE ... URI
}

int ConfigParser::validateAutoindexArgs(void)
{
	// autoindex (on | off)
}

int ConfigParser::validateIndexArgs(void)
{
	//	index FILE_NAME ...
}

int ConfigParser::validateReturnArgs(void)
{
	//	return HTTP_CODE [body]
	//	return HTTP_CODE URI
}

int ConfigParser::validateOpeningBracketArgs(void)
{
	//	{
}

int ConfigParser::validateClosingBracketArgs(void)
{
	//	}
}

*/
