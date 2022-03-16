#include "Server.hpp"

int Server::start()
{
	
	_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

}

Server::Server()
{
	
	timeout.tv_sec  = 60;
	timeout.tv_usec = 0;
}

Server::~Server()
{
	close(listen_socket);
}