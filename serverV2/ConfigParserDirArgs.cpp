#include "webserv.hpp"
#include "ConfigParser.hpp"

int		ConfigParser::validateServerArgs(void)
{
	// ALLOCATES NEW VirtualServer
	//	TO BE STORED IN
	//	this->_curVS->

	if (this->_line.size() == 2
			&& this->_line[1] == ConfigParser::_directives[OPENING_BRACKET])
	{
		this->_curVS->reset();
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
		this->_curLoc->reset();
		this->_curLoc->setPrefix(this->_line[1]);
		this->_curVS->_locationIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateListenArgs(void)
{
	if (this->_line.size() == 2
			&& isNumber(_line[1])
			&& isValidPort(_line[1])
			&& this->_curVS->_listenPortIsSet == false)
	{
		this->_curVS->setListenPort(std::atoi(_line[1].c_str()));
		this->_curVS->_listenPortIsSet = true;
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
			&& this->_curVS->_serverNameIsSet == false)
	{
		this->_curVS->setServerName(this->_line[1]);
		this->_curVS->_serverNameIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateClientMaxBodySizeArgs(void)
{
	if (this->_line.size() == 2
			&& isValidClientMaxBodySize(this->_line[1])
			&& this->_curVS->_clientMaxBodySizeIsSet == false)
	{
		this->_curVS->setClientMaxBodySize(this->_line[1]);
		this->_curVS->_clientMaxBodySizeIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateRootArgs(void)
{
	if (this->_line.size() == 2
			&& isValidPrefix(&this->_line[1]) == true
			&& this->_curLoc->_rootIsSet == false)
	{
		this->_curLoc->setRoot(this->_line[1]);
		this->_curLoc->_rootIsSet = true;
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
		this->_curLoc->_errorPageIsSet = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateAutoindexArgs(void)
{
	if (this->_line.size() == 2
			&& isValidAutoindex(this->_line[1])
			&& this->_curLoc->_autoIndexIsSet == false)
	{
		if (this->_line[1] == "on")
			this->_curLoc->setAutoindex(true);
		else
			this->_curLoc->setAutoindex(false);
		this->_curLoc->_autoIndexIsSet = true;
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
			&& this->_curLoc->_indexIsSet == false)
	{
		/*
		it = this->_line.begin() + 1;
		ite = this->_line.end();
		this->_curLoc->setIndex(std::vector<std::string>(it, ite));
		*/
		this->_curLoc->setIndex(this->_line[1]);
		this->_curLoc->_indexIsSet = true;
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
			&& this->_curLoc->_returnIsSet == false)
	{
		this->_curLoc->setReturnCode(std::atoi(this->_line[1].c_str()));
		this->_curLoc->setReturnUri(this->_line[2]);
		this->_curLoc->_returnIsSet = true;
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
			&& this->_curLoc->_limitExceptIsSet == false)
	{
		this->_curLoc->setLimitExcept(this->_line);
		this->_curLoc->_limitExceptIsSet = true;
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
	if (this->_line.size() != 1)
		return (ARG_ERROR);
	if (this->_context == SERVER_CONTEXT)
	{
		//	this->_curLoc->validate();						   // return (false) if fail
		//	this->_curVS->noDuplicateLocation(*this->_curLoc); // return (false) if fail
		this->_curVS->addLocation(this->_curLoc->clone());
		return (true);
	}
	else if (this->_context == MAIN_CONTEXT)
	{
		//	this->_curVS->validate(); // return (false) if validate() failed
		this->_portsMap[this->_curVS->getListenPort()].addVS(this->_curVS->clone());	
		this->_portsMap[this->_curVS->getListenPort()].port_number
			= this->_curVS->getListenPort();
		return (true);
	}
	return (true);
}