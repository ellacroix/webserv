#include "VirtualServer.hpp"

VirtualServer::VirtualServer(void) :
	_listenPort(-1),
	_clientMaxBodySize(0),
	_serverNameIsSet(false),
	_listenPortIsSet(false),
	_clientMaxBodySizeIsSet(false),
	_locationIsSet(false)
{
	return ;
}

VirtualServer::~VirtualServer(void)
{
	return ;
}

void	VirtualServer::setListenPort(int n)
{
	this->_listenPort = n;
}

void	VirtualServer::reset(void)
{
	this->_listenPort = -1;
	this->_serverName = "";
	this->_clientMaxBodySize = 0;

	this->_locationMap = std::map<std::string, Location*>();

	this->_serverNameIsSet = false;
	this->_listenPortIsSet = false;
	this->_clientMaxBodySizeIsSet = false;
	this->_locationIsSet = false;
}

void	VirtualServer::display(void) const
{
	std::cout << "_listenPort\t\t=\t" << this->_listenPort << std::endl;
	std::cout << "_serverName\t\t=\t\"" << this->_serverName << "\"" << std::endl;
	std::cout << "_clientMaxBodySize\t=\t" << this->_clientMaxBodySize << std::endl;
	std::cout << std::boolalpha;
	std::cout << "_serverNameIsSet\t=\t" << this->_serverNameIsSet << std::endl;
	std::cout << "_listenPortIsSet\t=\t" << this->_serverNameIsSet << std::endl;
	std::cout << "_clientMaxBodySizeIsSet\t=\t" << this->_serverNameIsSet << std::endl;
	std::cout << "_locationIsSet\t\t=\t" << this->_serverNameIsSet << std::endl;
}
