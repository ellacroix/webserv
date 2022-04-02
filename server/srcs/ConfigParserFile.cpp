#include "webserv.hpp"
#include "ConfigParser.hpp"

void				ConfigParser::parse(char *arg)
{
	std::ifstream		ifs;
	int					ret;

	ifs.open(arg);
	if (ifs.good() == false)
	{
		std::cerr << "webserv\t- ERROR - failed to open(2) \""
			<< arg << "\" file." << std::endl;
		exit(1);
	}
	this->_dir = DIR_ERROR;
	while (std::getline(ifs, this->_curLine))
	{
		this->_lineN++;
		if (this->hasContent() == true && this->isComment() == false)
		{
			this->splitLineIntoTokens();
			this->displayLine(this->_line);
			this->_dir = this->validateDirective();
			if (this->_dir == DIR_ERROR)
			{
				std::cerr << "webserv\t- ERROR line " << this->_lineN << " - \"" 
					<< this->_line[0] << "\" is not a valid directive."
					<< std::endl;
				exit(1);
			}
			if (this->validateContext() == false)
			{
				std::cerr << "webserv\t- ERROR line " << this->_lineN << " - \"" 
					<< this->_line[0]
					<< "\" directive is not valid in this context: "
					<< ConfigParser::_contexts[this->_context]
					<< "." << std::endl;
				exit(1);
			}
			ret = validateArguments();
			if (ret == ARG_ERROR || ret == LOC_BLCK_ERROR
					|| ret == SERV_BLCK_ERROR)
			{
				std::cerr << "webserv\t- ERROR line " << this->_lineN;
				if (ret == ARG_ERROR)
					std::cerr << " - \"" << this->_line[0]
						<< "\" has invalid arguments." << std::endl;
				else if (ret == LOC_BLCK_ERROR)
					std::cerr << " - location block needs at least "
						<< "1 \"root\" directive or 1 \"return\" directive."
						<< std::endl;
				else if (ret == SERV_BLCK_ERROR)
					std::cerr << " - server block needs at least "
						<< "1 \"location\" block and 1 \"listen\" directive."
						/*<< "or one \"return\" directive"*/ << std::endl;
				exit(1);
			}
		}
	}
//	this->displayPortsMap();
	this->displayPortsList();
//	this->makeListFromMap();
	if (this->validate() == false)
	{
		std::cerr << "webserv\t- ERROR - there is no Port to be listened to."
			<< std::endl;
		exit(1);
	}
	ifs.close();
}

void				ConfigParser::splitLineIntoTokens(void)
{
	char				*tok;
	char				*c_str;

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
	return (DIR_ERROR);
}

int					ConfigParser::validateArguments(void)
{
	int	ret;

	switch (this->_dir)
	{
		case SERVER :
			ret = this->validateServerArgs();
			break ;
		case LOCATION :
			ret = this->validateLocationArgs();
			break ;
		case LISTEN :
			ret = this->validateListenArgs();
			break ;
		case SERVER_NAME :
			ret = this->validateServerNameArgs();
			break ;
		case CLIENT_MAX_BODY_SIZE :
			ret = this->validateClientMaxBodySizeArgs();
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
		case INDEX :
			ret = this->validateIndexArgs();
			break ;
		case RETURN :
			ret = this->validateReturnArgs();
			break ;
		case LIMIT_EXCEPT :
			ret = this->validateLimitExceptArgs();
			break ;
		case OPENING_BRACKET :
			ret = this->validateOpeningBracketArgs();
			break ;
		case CLOSING_BRACKET :
			ret = this->validateClosingBracketArgs();
			break ;
	}
	return (ret);
}

bool        ConfigParser::validateContext(void)
{
	if (this->_dir == SERVER
			&& this->_context == MAIN_CONTEXT)
	{
		this->_context = SERVER_CONTEXT;
		return (true);
	}
	else if (this->_dir == LOCATION
			&& this->_context == SERVER_CONTEXT)
	{
		this->_context = LOCATION_CONTEXT;
		return (true);
	}
	//	else if (this->_dir >= LISTEN && this->_dir <= RETURN
	else if (this->_dir >= LISTEN && this->_dir <= CLIENT_MAX_BODY_SIZE
			&& this->_context == SERVER_CONTEXT)
		return (true);
	else if (this->_dir >= ROOT && this->_dir <= LIMIT_EXCEPT
			&& this->_context == LOCATION_CONTEXT)
		return (true);
	else if (this->_dir == CLOSING_BRACKET
			&& (this->_context == SERVER_CONTEXT
				|| this->_context == LOCATION_CONTEXT))
	{
		this->_context = this->_context - 1;
		return (true);
	}
	else
		return (false);
}

bool	ConfigParser::hasContent(void) const
{
	size_t	i;

	if (this->_curLine.empty() == true)
		return (false);
	i = 0;
	while (this->_curLine[i])
	{
		if (isspace(this->_curLine[i]) == false)
			return (true);
		i++;
	}
	return (false);
}

bool	ConfigParser::isComment(void) const
{
	size_t	i;

	i = 0;
	while (std::isspace(this->_curLine[i]))
		i++;
	if (_curLine[i] == '#')
		return (true);
	return (false);
}

bool    ConfigParser::noDuplicateErrorPage(void)
{
	int                                     httpCode;
	std::map<int, std::string>::iterator    ite;
	size_t                                  i;
	size_t                                  size;

	ite = this->_curLoc->getErrorPage().end();
	i = 1;
	size = this->_line.size();
	while (i < size - 1)
	{
		httpCode = std::atoi(this->_line[i].c_str());
		if (this->_curLoc->getErrorPage().find(httpCode) != ite)
			return (false);
		this->_curLoc->getErrorPage()[httpCode] = this->_line[size - 1];
		i++;
	}
	return (true);
}
