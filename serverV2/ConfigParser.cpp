#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void) :
	_context(MAIN_CONTEXT),
	_dir(-1)
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

void						ConfigParser::displayLine(std::vector<std::string> v) const
{
	std::vector<std::string>::const_iterator	it = v.begin();
	std::vector<std::string>::const_iterator	ite = v.end();

	while (it != ite)
	{
		std::cout << "\"" << *it << "\"\t";
		it++;
	}
}

void						ConfigParser::displayLines(void) const
{
	std::vector<std::vector<std::string> >::const_iterator	it = this->_lines.begin();
	std::vector<std::vector<std::string> >::const_iterator	ite = this->_lines.end();

	while (it != ite)
	{
		//std::cout << "\"" << *it << "\"" << std::endl;
		this->displayLine(*it);
		std::cout << std::endl;
		it++;
	}
}

const char *	ConfigParser::_directives[N_DIR] =
{
	"server", "listen", "server_name", "client_max_body_size",
	"location", "root", "error_page", "autoindex", "index",
	"return", "{", "}"
};

