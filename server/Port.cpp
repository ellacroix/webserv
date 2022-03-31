#include "Port.hpp"
#include <vector>
#include <arpa/inet.h>

Port::Port(void) :
	on(1),
	kill_port(false),
	opened(false)
{
	return ;
}

Port::Port(int port) :
	on(1),
	kill_port(false),
	opened(false)
{
	port_number = port;
	return ;
}

Port::~Port(void)
{
	std::list<VirtualServer*>::iterator	it;
	std::list<VirtualServer*>::iterator	ite;

	it = this->_VSList.begin();
	ite = this->_VSList.end();
	while (it != ite)
	{
		if (*it != NULL)
		{
			delete *it;
			*it = NULL;
		}
		it++;
	}
	return ;

	if (opened == true)
		close(this->listen_socket);
}

int Port::start(void)
{
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket < 0){
		perror("socket() failed");
		exit(-1);
	}
	this->opened = true;

	//Prevent “Address already in use" error by the OS
	int rc = setsockopt(listen_socket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0){
		perror("setsockopt() failed");
		close(listen_socket);
		exit(-1);
	}

	//Basic bind to a port
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	//server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(this->port_number);
	addr_len = sizeof(server_address);
	rc = bind(listen_socket, (struct sockaddr *)&server_address, addr_len);
	if (rc < 0){
		perror("bind() failed");
		close(listen_socket);
		exit(-1);
	}

	//Set O_NONBLOCK flag on the listening socket, so it transmits to sockets created with accept()
	rc = fcntl(listen_socket, F_SETFL, O_NONBLOCK);
	if (rc < 0){
		perror("fcntl() failed");
		close(listen_socket);
		exit(-1);
	}

	//Set the socket as listen listening for any connections
	listen(listen_socket, 20);
	if (rc < 0){
		perror("listen() failed");
		close(listen_socket);
		exit(-1);
	}

	return 0;
}

//	UTILITIES
void	Port::addVS(VirtualServer * newVS)
{
	this->_VSList.push_back(newVS);
	this->port_number = newVS->getListenPort();
}

void	Port::display(void) const
{
	std::list<VirtualServer*>::const_iterator	it;
	std::list<VirtualServer*>::const_iterator	ite;

	it = this->_VSList.begin();
	ite = this->_VSList.end();
	while (it != ite)
	{
		(*it)->display();
		it++;
	}
}

bool	Port::validate(void) const
{
	if (this->_VSList.empty() == true)
	{
		std::cerr << "webserv - ERROR - a Port has no VirtualServer configured."
			<< std::endl;
		return (false);
	}
	return (true);
}
