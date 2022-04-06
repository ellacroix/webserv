#include "VirtualServer.hpp"

//	CONSTRUCTORS / DESTRUCTOR
VirtualServer::VirtualServer(void) :
	_listenPort(-1),
	//	_serverName(""),
	_serverNameIsSet(false),
	_listenPortIsSet(false),
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
	//	_clientMaxBodySize(src._clientMaxBodySize),
	_locationMap(src._locationMap),		// !!! DEEP COPY
	_serverNameIsSet(src._serverNameIsSet),
	_listenPortIsSet(src._listenPortIsSet),
	_locationIsSet(src._locationIsSet)
	//	_clientMaxBodySizeIsSet(src._clientMaxBodySizeIsSet)
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

//	GETTERS
int		VirtualServer::getListenPort(void) const
{
	return (this->_listenPort);
}

void	VirtualServer::reset(void)
{
	this->_listenPort = -1;
	//	this->_serverName = "";
	this->_serverName = std::vector<std::string>();
	//	this->_clientMaxBodySize = 0;

	this->_locationMap = std::map<std::string, Location*>();

	this->_serverNameIsSet = false;
	this->_listenPortIsSet = false;
	//	this->_clientMaxBodySizeIsSet = false;
	this->_locationIsSet = false;
}

void	VirtualServer::display(void) const
{
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
