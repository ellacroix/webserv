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
		this->_tmpVS._locationIsSet = true;
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

int ConfigParser::validateClientMaxBodySizeArgs(void)
{
	if (this->_line.size() == 2
			&& isValidClientMaxBodySize(this->_line[1])
			&& this->_tmpVS._clientMaxBodySizeIsSet == false)
	{
		this->_tmpVS.setClientMaxBodySize(this->_line[1]);
		this->_tmpVS._clientMaxBodySizeIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateRootArgs(void)
{
	if (this->_line.size() == 2
			&& isValidPrefix(&this->_line[1]) == true
			&& this->_tmpLoc._rootIsSet == false)
	{
		this->_tmpLoc.setRoot(this->_line[1]);
		this->_tmpLoc._rootIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateErrorPageArgs(void)
{
	if (this->_line.size() > 2
			&& isValidErrorPage(this->_line) == true
			&& this->noDuplicateErrorPage() == true)
	{
		this->_tmpLoc._errorPageIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateAutoindexArgs(void)
{
	if (this->_line.size() == 2
			&& isValidAutoindex(this->_line[1])
			&& this->_tmpLoc._autoIndexIsSet == false)
	{
		if (this->_line[1] == "on")
			this->_tmpLoc.setAutoindex(true);
		else
			this->_tmpLoc.setAutoindex(false);
		this->_tmpLoc._autoIndexIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateIndexArgs(void)
{
//	std::vector<std::string>::iterator	it;
//	std::vector<std::string>::iterator	ite;

	if (this->_line.size() >= 2
//			&& isValidIndex(this->_line)
			&& isValidIndex(this->_line[1])
			&& this->_tmpLoc._indexIsSet == false)
	{
		/*
		it = this->_line.begin() + 1;
		ite = this->_line.end();
		this->_tmpLoc.setIndex(std::vector<std::string>(it, ite));
		*/
		this->_tmpLoc.setIndex(this->_line[1]);
		this->_tmpLoc._indexIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateReturnArgs(void)
{
//	PROBABLY A LOT OF MUTUAL DIRECTIVE EXCLUSIONS IN THE CASE OF RETURN
//	PATTERN : return HTTP_CODE URI
	if (this->_line.size() == 3
			&& isValidReturn(this->_line)
			&& this->_tmpLoc._returnIsSet == false)
	{
		this->_tmpLoc.setReturnCode(std::atoi(this->_line[1].c_str()));
		this->_tmpLoc.setReturnUri(this->_line[2]);
		this->_tmpLoc._returnIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
	// OTHER PATTERN : return HTTP_CODE [text] => WHAT IS [text] ?
}

int	ConfigParser::validateLimitExceptArgs(void)
{
//	limit_except method ...
	if (this->_line.size() >= 2 && this->_line.size() <= 4
			&& isValidLimitExcept(this->_line)
			&& this->_tmpLoc._limitExceptIsSet == false)
	{
		this->_tmpLoc.setLimitExcept(this->_line);
		this->_tmpLoc._limitExceptIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateOpeningBracketArgs(void)
{
	if (this->_line.size() == 1)
		return (true);
	return (ARG_ERROR);
}

int ConfigParser::validateClosingBracketArgs(void)
{
	if (this->_line.size() == 1)
		return (true);
	return (ARG_ERROR);
}

