#include "webserv.hpp"
#include "ConfigParser.hpp"

int		ConfigParser::validateServerArgs(void)
{
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
	if (this->_line.size() == 3
			&& isValidPrefix(&this->_line[1])
			&& this->_line[2] == ConfigParser::_directives[OPENING_BRACKET])
	{
		this->_curLoc->reset();
		*(this->_curLoc) = *(this->_defLocPtr);
		this->_curLoc->resetIsDefBooleans();
		this->_curLoc->setPrefix(this->_line[1]);
		this->_curVS->_location_is_set = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateListenArgs(void)
{
	if (this->_line.size() == 2
			&& isNumber(_line[1])
			&& isValidPort(_line[1])
			&& this->_curVS->_listen_port_is_set == false)
	{
		this->_curVS->setListenPort(std::atoi(_line[1].c_str()));
		this->_curVS->_listen_port_is_set = true;
		return (true);
	}
	//	TWO MORE POTENTIAL PATTERNS 
	//	address:port
	//	address (port = 80)
	return (ARG_ERROR);
}

int ConfigParser::validateServerNameArgs(void)
{
	//	if (this->_line.size() == 2
	//			&& isValidDomainName(this->_line[1])
	//			&& this->_curVS->_server_name_is_set == false)
	//	{
	//		this->_curVS->setServerName(this->_line[1]);
	//		this->_curVS->_server_name_is_set = true;
	//		return (true);
	//	}
	if (this->_line.size() >= 2
			&& areValidDomainNames(this->_line)
			&& this->_curVS->_server_name_is_set == false)
	{
		this->_curVS->setServerName(this->_line);
		this->_curVS->_server_name_is_set = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateClientMaxBodySizeArgs(void)
{
	if (this->_line.size() == 2
			&& isValidClientMaxBodySize(this->_line[1]))
	{
		/*
		if (this->_defLocPtr->_client_max_body_size_is_set == true)
			return (ALRDY_SET_ERROR);
		this->_defLocPtr->setClientMaxBodySize(this->_line[1]);
		this->_defLocPtr->_client_max_body_size_is_set = true;
		*/
		if (this->_curVS->_client_max_body_size_is_set == true)
			return (ALRDY_SET_ERROR);
		this->_curVS->setClientMaxBodySize(this->_line[1]);
		this->_curVS->_client_max_body_size_is_set = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateRootArgs(void)
{
	if (this->_line.size() == 2
			&& isValidPrefix(&this->_line[1]) == true
			//			&& this->_curLoc->_rootIsSet == false)
		)
		{
			if (this->_context == LOCATION_CONTEXT)
			{
				if (this->_curLoc->_rootIsSet == true)
					return (ALRDY_SET_ERROR);
				this->_curLoc->setRoot(this->_line[1]);
				this->_curLoc->_rootIsSet = true;
			}
			else if (this->_context == SERVER_CONTEXT)
			{
				if (this->_defLocPtr->_rootIsSet == true)
					return (ALRDY_SET_ERROR);
				this->_defLocPtr->setRoot(this->_line[1]);
				this->_defLocPtr->_rootIsSet = true;
			}
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
		/*
		if (this->_context == LOCATION_CONTEXT)
			this->_curLoc->_error_page_is_set = true;
		else if (this->_context == SERVER_CONTEXT)
		*/
		this->_curVS->_error_page_is_set = true;
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateAutoindexArgs(void)
{
	if (this->_line.size() == 2
			&& isValidAutoindex(this->_line[1])
	   )
	{
		if (this->_context == LOCATION_CONTEXT)
		{
			if (this->_curLoc->_autoIndexIsSet == true)
				return (ALRDY_SET_ERROR);
			if (this->_line[1] == "on")
				this->_curLoc->setAutoindex(true);
			else
				this->_curLoc->setAutoindex(false);
			this->_curLoc->_autoIndexIsSet = true;
		}
		else if (this->_context == SERVER_CONTEXT)
		{
			if (this->_defLocPtr->_autoIndexIsSet == true)
				return (ALRDY_SET_ERROR);
			if (this->_line[1] == "on")
				this->_defLocPtr->setAutoindex(true);
			else
				this->_defLocPtr->setAutoindex(false);
			this->_defLocPtr->_autoIndexIsSet = true;
		}
		return (true);
	}
	return (ARG_ERROR);
}

int ConfigParser::validateIndexArgs(void)
{
	//	std::vector<std::string>::iterator	it;
	//	std::vector<std::string>::iterator	ite;

	if (this->_line.size() >= 2
			&& areValidIndexes(this->_line))
		//			&& this->_curLoc->_indexIsSet == false)
	{
		if (this->_context == LOCATION_CONTEXT)
		{
			this->_curLoc->setIndex(this->_line);
			this->_curLoc->_indexIsSet = true;
		}
		else if (this->_context == SERVER_CONTEXT)
		{
			this->_defLocPtr->setIndex(this->_line);
			this->_defLocPtr->_indexIsSet = true;
		}
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
			//			&& this->_curLoc->_returnIsSet == false)
		)
		{
			if (this->_context == LOCATION_CONTEXT)
			{
				this->_curLoc->setReturnCode(std::atoi(this->_line[1].c_str()));
				this->_curLoc->setReturnUri(this->_line[2]);
				this->_curLoc->_returnIsSet = true;
			}
			else if (this->_context == SERVER_CONTEXT)
			{
				this->_defLocPtr->setReturnCode(std::atoi(this->_line[1].c_str()));
				this->_defLocPtr->setReturnUri(this->_line[2]);
				this->_defLocPtr->_returnIsSet = true;
			}
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
	Port	*port;

	if (this->_line.size() != 1)
		return (ARG_ERROR);
	if (this->_context == SERVER_CONTEXT)		// (== LOCATION_CONTEXT)
	{
		this->setDefLocTrueBoolsInCurLoc();
		if (this->_curLoc->validate() == false)
			return (LOC_BLCK_ERROR);
		//	this->_curVS->noDuplicateLocation(*this->_curLoc);
		this->_curVS->addLocation(this->_curLoc->clone());
		return (true);
	}
	else if (this->_context == MAIN_CONTEXT)	// (== SERVER_CONTEXT)
	{
		this->_defLocPtr->reset();
		if (this->_curVS->validate() == false)
			return (SERV_BLCK_ERROR);
		port = this->findPortInList(this->_curVS->getListenPort());
		if (port == NULL)
		{
			port = new Port();
			this->_ports_list.push_back(port);
		}
		port->addVS(this->_curVS->clone());
		return (true);
	}
	return (true);
}

int	ConfigParser::validateCgiArgs(void)
{
//	"cgi"	".ext"	"/absolute/path/to/cgi"
	if (this->_line.size() != 3)
		return (false);
	if (this->_line[1].find('/') != std::string::npos
			|| this->_line[1].find("..") != std::string::npos
			|| this->_line[1][0] != '.')
		return (false);
	if (this->_line[2][0] != '/'
			|| this->_line[2][this->_line[2].length() - 1] == '/'
			|| this->_line[2].length() == 1)
		return (false);
	if (this->_curVS->_cgi_is_set == true)
		return (ALRDY_SET_ERROR);
	this->_curVS->setCgiExtension(this->_line[1]);
	this->_curVS->setCgiPath(this->_line[2]);
	this->_curVS->_cgi_is_set = true;
	return (true);
}

int	ConfigParser::validateUploadFldArgs(void)
{
	if (this->_line.size() != 2)
		return (ARG_ERROR);
	if (isValidPrefix(&this->_line[1]) == false)
		return (ARG_ERROR);
	this->_curLoc->setUploadFld(this->_line[1]);
	this->_curLoc->_uploadFldIsSet = true;
	return (true);
}
