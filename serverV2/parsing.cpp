#include "webserv.hpp"
#include "ConfigParser.hpp"

void				ConfigParser::splitLineIntoTokens(void)
{
	char				*tok;
	char				*c_str;

	if (this->_curLine.empty() == false)
	{
		this->_line.clear();
		c_str = const_cast<char *>(this->_curLine.c_str());
		tok = std::strtok(c_str, " \t");
		while (tok != NULL)
		{
			this->_line.push_back(tok);
			tok = strtok(NULL, " \t");	
		}
		this->_lines.push_back(this->_line);
	}
}

int					ConfigParser::validateDirective(void)
{
	int	i;

	i = 0;
	while (i < N_DIR)
	{
		if (this->_line[0] == ConfigParser::_directives[i])
			return (i);
		i++;
	}
	return (-1);
}

int					ConfigParser::validateArguments(void)
{
	int	ret;

	switch (this->_dir)
	{
		case SERVER :
			ret = this->validateServerArgs();
			break ;
		/*
		case LISTEN :
			ret = this->validateListenArgs();
			break ;
		case SERVER_NAME :
			ret = this->validateServerNameArgs();
			break ;
		case CLIENT_MAX_BODY_SIZE :
			ret = this->validateClientMaxBodySizeArgs();
			break ;
		case LOCATION :
			ret = this->validateLocationArgs();
			break ;
		case ROOT :
			ret = this->validateRootArgs();
			break ;
		case ERROR_PAGE :
			ret = this->validateErrorPageArgs();
			break ;
		case AUTOINDEX :
			ret = this->validateAutoindexArgs();
			break ;
		case AUTOINDEX :
			ret = this->validateIndexArgs();
			break ;
		case RETURN :
			ret = this->validateReturnArgs();
			break ;
		case OPENING_BRACKET :
			ret = this->validateOpeningBracketArgs();
			break ;
		case CLOSING_BRACKET :
			ret = this->validateClosingBracketArgs();
			break ;
		*/
	}
	return (ret);
}

std::list<Port *>	ConfigParser::parse(char *arg)
{
	std::ifstream		ifs;
	std::list<Port *>	portsList;

	ifs.open(arg);
	if (ifs.good() == false)
	{
		std::cerr << "webserv\t- ERROR - failed to open(2) \""
			<< arg << "\" file." << std::endl;
		exit(1);
	}

	while (std::getline(ifs, this->_curLine))
	{
		this->splitLineIntoTokens();
		this->_dir = this->validateDirective();
		if (this->_dir == DIR_ERROR)
		{
			std::cerr << "webserv\t- ERROR - \"" << this->_line[0]
				<< "\" is not a valid directive." << std::endl;
			exit(1);
		}
		//	VALIDATE ARGUMENTS
		this->validateArguments();
		//	VALIDATE CONTEXT
		//	POTENTIAL CONTEXT SWITCH
	}

	this->displayLines();

	ifs.close();
	return (portsList);
}
