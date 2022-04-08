#include "Response.hpp"

VirtualServer *	Response::findVirtualServer(std::string & s)
{
	t_VSListCIt							it;
	t_VSListCIt							ite;
	std::vector<std::string>::iterator	serverNameEnd;

	it = this->client->parent_port->_VSList.begin();
	ite = this->client->parent_port->_VSList.end();
	while (it != ite)
	{
		serverNameEnd = (*it)->getServerName().end();
		if (std::find((*it)->getServerName().begin(),
					(*it)->getServerName().end(), s) != serverNameEnd)
			return (*it);
		it++;
	}
	return (this->client->parent_port->_VSList.front());
}
