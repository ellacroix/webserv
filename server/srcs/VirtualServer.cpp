#include "VirtualServer.hpp"

//	CONSTRUCTORS / DESTRUCTOR
VirtualServer::VirtualServer(void) :
	_listen_port(-1),
	_client_max_body_size(0),
	_server_name_is_set(false),
	_listen_port_is_set(false),
	_client_max_body_size_is_set(false),
	_cgi_is_set(false),
	_error_page_is_set(false),
	_location_is_set(false)
{
	return ;
}

VirtualServer::~VirtualServer(void)
{
	std::map<std::string, Location*>::iterator	it;
	std::map<std::string, Location*>::iterator	ite;

	it = this->_location_map.begin();
	ite = this->_location_map.end();
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
	_listen_port(src._listen_port),
	_server_name(src._server_name),
	_client_max_body_size(src._client_max_body_size),
	_cgi_extension(src._cgi_extension),
	_cgi_path(src._cgi_path),
	_error_page(src._error_page),
	_location_map(src._location_map),
	_server_name_is_set(src._server_name_is_set),
	_listen_port_is_set(src._listen_port_is_set),
	_client_max_body_size_is_set(src._client_max_body_size_is_set),
	_cgi_is_set(src._cgi_is_set),
	_error_page_is_set(src._error_page_is_set),
	_location_is_set(src._location_is_set)
{
	return ;
}

//	SETTERS
void	VirtualServer::setListenPort(int n)
{
	this->_listen_port = n;
}

//void	VirtualServer::setServerName(std::string s)
//{
//	this->_server_name = s;
//}

void	VirtualServer::setServerName(std::vector<std::string> v)
{
	size_t	i;
	size_t	size;

	i = 1;
	size = v.size();
	while (i < size)
	{
		this->_server_name.push_back(v[i]);
		i++;
	}
}

void    VirtualServer::setClientMaxBodySize(std::string s)
{
    char    *endPtr;

    this->_client_max_body_size = std::strtoul(s.c_str(), &endPtr, 10);
    if (*endPtr == 'k')
        this->_client_max_body_size *= K;
    if (*endPtr == 'm')
        this->_client_max_body_size *= M;
}

void    VirtualServer::setCgiExtension(std::string s)
{
	this->_cgi_extension = s;
}

void    VirtualServer::setCgiPath(std::string s)
{
	this->_cgi_path = s;
}

//	GETTERS
int								VirtualServer::getListenPort(void) const
{
	return (this->_listen_port);
}

std::map<int, std::string> &	VirtualServer::getErrorPage(void)
{
	return (this->_error_page);
}

std::vector<std::string> &		VirtualServer::getServerName(void)
{
	return (this->_server_name);
}

/*
std::vector<std::string> &		VirtualServer::getLimitExcept(void)
{
	return (this->_limitExcept);
}
*/

std::map<std::string, Location*> &	VirtualServer::getLocationMap(void)
{
	return (this->_location_map);
}

unsigned int						VirtualServer::getClientMaxBodySize(void)
{
	return (this->_client_max_body_size);
}

std::string							VirtualServer::getCgiExtension(void)
{
	return (this->_cgi_extension);
}

std::string							VirtualServer::getCgiPath(void)
{
	return (this->_cgi_path);
}

//	UTILITIES
void	VirtualServer::reset(void)
{
	this->_listen_port = -1;
	this->_server_name = std::vector<std::string>();
	this->_client_max_body_size = 0;
	this->_cgi_extension = "";
	this->_cgi_path = "";
	this->_error_page = std::map<int, std::string>();

	this->_location_map = std::map<std::string, Location*>();

	this->_server_name_is_set = false;
	this->_listen_port_is_set = false;
	this->_client_max_body_size_is_set = false;
	this->_cgi_is_set = false;
	this->_error_page_is_set = false;
	this->_location_is_set = false;
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

	std::cout << "\t_listen_port\t\t=\t" << this->_listen_port << std::endl;
//	std::cout << "\t_server_name\t\t=\t\"" << this->_server_name << "\"" << std::endl;

	if (this->_server_name.empty() == true)
		std::cout << "\t_server_name[  ]\t=\tempty" << std::endl;
	else
	{
		v_it = this->_server_name.begin();
		v_ite = this->_server_name.end();
		i = 0;
		while (v_it != v_ite)
		{
			std::cout << "\t_server_name[" << i << "]\t\t=\t\""
				<< this->_server_name[i] << "\"" << std::endl;
			i++;
			v_it++;
		}
	}
	std::cout << "\t_client_max_body_size\t=\t"
		<< this->_client_max_body_size << std::endl;
	std::cout << "\t_cgi_extention\t\t=\t\""
		<< this->_cgi_extension << "\"" << std::endl;
	std::cout << "\t_cgi_path\t\t=\t\""
		<< this->_cgi_path << "\"" << std::endl;
    if (this->_error_page.empty() == true)
        std::cout << "\t_error_page[ ]\t\t=\tempty" << std::endl;
    else
    {
        map_it = this->_error_page.begin();
        map_ite = this->_error_page.end();
        while (map_it != map_ite)
        {
            std::cout << "\t_error_page[" << (*map_it).first << "]\t=\t\""
                << (*map_it).second << "\"" << std::endl;
            map_it++;
        }
    }


	std::cout << std::boolalpha;
	std::cout << "\t_server_name_is_set\t=\t" << this->_server_name_is_set << std::endl;
	std::cout << "\t_listen_port_is_set\t=\t" << this->_listen_port_is_set << std::endl;
	std::cout << "\t_cli_mx_bd_sz_is_set\t=\t"
		<< this->_client_max_body_size_is_set << std::endl;
	std::cout << "\t_cgi_is_set\t\t=\t" << this->_cgi_is_set << std::endl;
	std::cout << "\t_error_page_is_set\t=\t" << this->_error_page_is_set << std::endl;
	std::cout << "\t_location_is_set\t=\t" << this->_location_is_set << std::endl;

	if (this->_location_map.empty() == true)
		std::cout << "\t_location_map\t\t=\tempty" << std::endl;
	else
	{
		it = this->_location_map.begin();
		ite = this->_location_map.end();
		while (it != ite)
		{
			std::cout << "\t_location_map[" << it->first << "] :" << std::endl;
			it->second->display();
			it++;
		}
	}
}

void	VirtualServer::addLocation(Location * new_location)
{
	this->_location_map[new_location->getPrefix()] = new_location;
}

VirtualServer *		VirtualServer::clone(void) const
{
	VirtualServer	*ptr;

	ptr = new VirtualServer(*this);
	return (ptr);
}

bool				VirtualServer::validate(void) const
{
	return (this->_listen_port_is_set == true
			&& this->_location_map.empty() == false);
}
