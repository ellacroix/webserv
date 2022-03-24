#include "webserv.hpp"
#include "ConfigParser.hpp"

int ConfigParser::validateServerArgs(void)
{
	if (this->_line.size() == 1
		|| (this->_line.size() == 2
			&& this->_line[1] == ConfigParser::_directives[OPENING_BRACKET]))
		return (true);
	return (ARG_ERROR);
}

/*
int ConfigParser::validateListenArgs(void)
{
}

int ConfigParser::validateServerNameArgs(void)
{
}

int ConfigParser::validateClientMaxBodySizeArgs(void)
{
}

int ConfigParser::validateLocationArgs(void)
{
}

int ConfigParser::validateRootArgs(void)
{
}

int ConfigParser::validateErrorPageArgs(void)
{
}

int ConfigParser::validateAutoindexArgs(void)
{
}

int ConfigParser::validateIndexArgs(void)
{
}

int ConfigParser::validateReturnArgs(void)
{
}

int ConfigParser::validateOpeningBracketArgs(void)
{
}

int ConfigParser::validateClosingBracketArgs(void)
{
}

*/

