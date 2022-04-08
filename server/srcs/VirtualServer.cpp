#include "VirtualServer.hpp"

//	CONSTRUCTORS / DESTRUCTOR
VirtualServer::VirtualServer(void) :
	_listenPort(-1),
	_clientMaxBodySize(0),
	_serverNameIsSet(false),
	_listenPortIsSet(false),
	_clientMaxBodySizeIsSet(false),
	_errorPageIsSet(false),
	_locationIsSet(false)
{
	return ;
}

VirtualServer::~VirtualServer(void)
{
	std::map<std::string, Location*>::iterator	it;
	std::map<std::string, Location*>::iterator	ite;

	it = this->_locationMap.begin();
	ite = this->_locationMap.end();
	while (it != ite)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
		it++;
	}
	return ;
}

VirtualServer::VirtualServer(VirtualServer const & src) :
	_listenPort(src._listenPort),
	_serverName(src._serverName),
	_clientMaxBodySize(src._clientMaxBodySize),
	_errorPage(src._errorPage),
	_locationMap(src._locationMap),
	_serverNameIsSet(src._serverNameIsSet),
	_listenPortIsSet(src._listenPortIsSet),
	_clientMaxBodySizeIsSet(src._clientMaxBodySizeIsSet),
	_errorPageIsSet(src._errorPageIsSet),
	_locationIsSet(src._locationIsSet)
{
	return ;
}

//	SETTERS
void	VirtualServer::setListenPort(int n)
{
	this->_listenPort = n;
}

//void	VirtualServer::setServerName(std::string s)
//{
//	this->_serverName = s;
//}

void	VirtualServer::setServerName(std::vector<std::string> v)
{
	size_t	i;
	size_t	size;

	i = 1;
	size = v.size();
	while (i < size)
	{
		this->_serverName.push_back(v[i]);
		i++;
	}
}

void    VirtualServer::setClientMaxBodySize(std::string s)
{
    char    *endPtr;

    this->_clientMaxBodySize = std::strtoul(s.c_str(), &endPtr, 10);
    if (*endPtr == 'k')
        this->_clientMaxBodySize *= K;
    if (*endPtr == 'm')
        this->_clientMaxBodySize *= M;
}

//	GETTERS
int								VirtualServer::getListenPort(void) const
{
	return (this->_listenPort);
}

std::map<int, std::string> &	VirtualServer::getErrorPage(void)
{
	return (this->_errorPage);
}

std::vector<std::string> &		VirtualServer::getServerName(void)
{
	return (this->_serverName);
}

std::vector<std::string> &		VirtualServer::getLimitExcept(void)
{
	return (this->_limitExcept);
}

//	UTILITIES
void	VirtualServer::reset(void)
{
	this->_listenPort = -1;
	this->_serverName = std::vector<std::string>();
	this->_clientMaxBodySize = 0;
	this->_errorPage = std::map<int, std::string>();

	this->_locationMap = std::map<std::string, Location*>();

	this->_serverNameIsSet = false;
	this->_listenPortIsSet = false;
	this->_clientMaxBodySizeIsSet = false;
	this->_locationIsSet = false;
}

void	VirtualServer::display(void) const
{
	std::map<int, std::string>::const_iterator			map_it;
	std::map<int, std::string>::const_iterator			map_ite;
	std::map<std::string, Location*>::const_iterator	it;
	std::map<std::string, Location*>::const_iterator	ite;
	std::vector<std::string>::const_iterator			v_it;
	std::vector<std::string>::const_iterator			v_ite;
	size_t												i;

	std::cout << "\t_listenPort\t\t=\t" << this->_listenPort << std::endl;
//	std::cout << "\t_serverName\t\t=\t\"" << this->_serverName << "\"" << std::endl;

	if (this->_serverName.empty() == true)
		std::cout << "\t_serverName[  ]\t\t=\tempty" << std::endl;
	else
	{
		v_it = this->_serverName.begin();
		v_ite = this->_serverName.end();
		i = 0;
		while (v_it != v_ite)
		{
			std::cout << "\t_serverName[" << i << "]\t\t=\t\""
				<< this->_serverName[i] << "\"" << std::endl;
			i++;
			v_it++;
		}
	}
	std::cout << "\t_clientMaxBodySize\t=\t"
		<< this->_clientMaxBodySize << std::endl;
    if (this->_errorPage.empty() == true)
        std::cout << "\t_errorPage[ ]\t\t=\tempty" << std::endl;
    else
    {
        map_it = this->_errorPage.begin();
        map_ite = this->_errorPage.end();
        while (map_it != map_ite)
        {
            std::cout << "\t_errorPage[" << (*map_it).first << "]\t\t=\t\""
                << (*map_it).second << "\"" << std::endl;
            map_it++;
        }
    }


	std::cout << std::boolalpha;
	std::cout << "\t_serverNameIsSet\t=\t" << this->_serverNameIsSet << std::endl;
	std::cout << "\t_listenPortIsSet\t=\t" << this->_listenPortIsSet << std::endl;
	std::cout << "\t_locationIsSet\t\t=\t" << this->_locationIsSet << std::endl;

	if (this->_locationMap.empty() == true)
		std::cout << "\t_locationMap\t\t=\tempty" << std::endl;
	else
	{
		it = this->_locationMap.begin();
		ite = this->_locationMap.end();
		while (it != ite)
		{
			std::cout << "\t_locationMap[" << it->first << "] :" << std::endl;
			it->second->display();
			it++;
		}
	}
}

void	VirtualServer::addLocation(Location * newLocation)
{
	this->_locationMap[newLocation->getPrefix()] = newLocation;
}

VirtualServer *		VirtualServer::clone(void) const
{
	VirtualServer	*ptr;

	ptr = new VirtualServer(*this);
	return (ptr);
}

bool				VirtualServer::validate(void) const
{
	return (this->_listenPortIsSet == true
			&& this->_locationMap.empty() == false);
}
