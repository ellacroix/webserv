#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void) :
	_context(MAIN_CONTEXT),
	_dir(-1),
	_lineN(0),
	_tmpVSIsStored(false),
	_tmpLocIsStored(false)
{
	return ;
}

ConfigParser::~ConfigParser(void)
{
	return ;
}

ConfigParser::ConfigParser(ConfigParser const & src)
{
	(void)src;
	return ;
}

ConfigParser	&ConfigParser::operator=(ConfigParser const & rhs)
{
	(void)rhs;
	return (*this);
}

int										ConfigParser::getContext(void) const
{
	return (this->_context);
}

std::vector<std::vector<std::string> >	ConfigParser::getLines(void) const
{
	return (this->_lines);
}

std::string								ConfigParser::getCurLine(void) const
{
	return (this->_curLine);
}

void		ConfigParser::displayLine(std::vector<std::string> v) const
{
	int	tab;
	std::vector<std::string>::const_iterator	it = v.begin();
	std::vector<std::string>::const_iterator	ite = v.end();

	tab = this->_context;
	while (tab--)
		std::cout << "\t";
	while (it != ite)
	{
		std::cout << "\"" << *it << "\"\t";
		it++;
	}
	std::cout << std::endl;
}

void	ConfigParser::displayLines(void) const
{
	std::vector<std::vector<std::string> >::const_iterator	it = this->_lines.begin();
	std::vector<std::vector<std::string> >::const_iterator	ite = this->_lines.end();

	while (it != ite)
	{
		this->displayLine(*it);
		it++;
	}
}

void	ConfigParser::displayContextSwitch(int newContext) const
{
	std::cout << "switching context : "
		<< ConfigParser::_contexts[this->_context]
		<< " to " 
		<< ConfigParser::_contexts[newContext]
		<< std::endl;
}

const char *	ConfigParser::_directives[N_DIR] =
{
	"server", "listen", "server_name", "client_max_body_size",
	"root", "error_page", "autoindex", "index",
	"return", "limit_except", "location", "{", "}"
};

const char *	ConfigParser::_contexts[N_CONTEXTS] =
{
	"MAIN_CONTEXT", "SERVER_CONTEXT", "LOCATION_CONTEXT",
	"SWITCHING_TO_SERVER", "SWITCHING_TO_LOCATION"
};
