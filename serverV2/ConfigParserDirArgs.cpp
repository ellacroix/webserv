#include "webserv.hpp"
#include "ConfigParser.hpp"

int		ConfigParser::validateServerArgs(void)
{
	// ALLOCATES NEW VirtualServer
	//	TO BE STORED IN
	//	this->_tmpVS;

	if (this->_line.size() == 2
			&& this->_line[1] == ConfigParser::_directives[OPENING_BRACKET])
	{
		this->_tmpVS.reset();
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateLocationArgs(void)
{
	// ALLOCATES NEW Location
	// TO BE STORED IN
	//		this->_portsMap[port]->_VSList->_locMap[prefix]

	if (this->_line.size() == 3
			&& isValidPrefix(&this->_line[1])
			&& this->_line[2] == ConfigParser::_directives[OPENING_BRACKET])
	{
		this->_tmpLoc.reset();
		this->_tmpLoc.setPrefix(this->_line[1]);
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateListenArgs(void)
{
	if (this->_line.size() == 2
			&& isNumber(_line[1])
			&& isValidPort(_line[1])
			&& this->_tmpVS._listenPortIsSet == false)
	{
		this->_tmpVS.setListenPort(std::atoi(_line[1].c_str()));
		this->_tmpVS._listenPortIsSet = true;
		// ALLOCATE NEW Port
		// TO BE STORED IN
		//		this->_portsList[port];
		return (true);
	}
	//	TWO MORE POTENTIAL PATTERNS 
	//	address:port
	//	address (port = 80)
	return (ARG_ERROR);
}

int ConfigParser::validateServerNameArgs(void)
{
	if (this->_line.size() == 2
			&& isValidDomainName(this->_line[1])
			&& this->_tmpVS._serverNameIsSet == false)
	{
		this->_tmpVS.setServerName(this->_line[1]);
		this->_tmpVS._serverNameIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

/*
   int ConfigParser::validateClientMaxBodySizeArgs(void)
   {
//	1* DIGITS 1(k | m)
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
