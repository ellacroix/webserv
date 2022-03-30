#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void) :
	_context(MAIN_CONTEXT),
	_dir(-1),
	_lineN(0),
	_tmpVSIsStored(false),
	_tmpLocIsStored(false),
	_curPort(NULL),
	_curVS(&this->_tmpVS),
	_curLoc(&this->_tmpLoc)
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

void	ConfigParser::displayPortsMap(void) const
{
	std::map<int, Port>::const_iterator	it;
	std::map<int, Port>::const_iterator	ite;
	int									i;

	it = this->_portsMap.begin();
	ite = this->_portsMap.end();
	i = 0;
	while (it != ite)
	{
		std::cout << "port[" << it->first << "] :" << std::endl;
		it->second.display();
		it++;
		i++;
	}
}

void	ConfigParser::makeListFromMap(void)
{
	std::map<int, Port>::iterator	it;
	std::map<int, Port>::iterator	ite;

	it = this->_portsMap.begin();
	ite = this->_portsMap.end();
	while (it != ite)
	{
		this->_portsList.push_back(&(it->second));
		it++;
	}
}

void	ConfigParser::startAllSockets(void)
{
	fd_set	master_reading_set;
	std::list<Port*>::iterator	it;
	std::list<Port*>::iterator	ite;

	FD_ZERO(&master_reading_set);
	it = this->_portsList.begin();
	ite = this->_portsList.end();
	while (it != ite)
	{
		(*it)->start();
		FD_SET((*it)->listen_socket, &master_reading_set);
		it++;
	}
}

std::list<Port*> &	ConfigParser::getPortsList(void)
{
	return (this->_portsList);
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