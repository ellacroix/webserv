#pragma once

#define BUFFER_SIZE 5000

#include <vector>
#include <map>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <list>

#include "Client.hpp"
#include "Virtual_Server.hpp"

class Port{

private:

public:
	int					port_number;
	int					listen_socket;
	struct sockaddr_in	server_address;
	int 				addr_len;
	int					max_sd;
	int					ret, on = 1;


	std::map<int, Client*> Clients;
	std::list<Virtual_Server*> Virtual_Servers;

	Port();
	Port(int port);
	~Port();

	int	start();

	void disconnectClient(int socket);

};