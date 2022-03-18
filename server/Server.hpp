#pragma once

#define PORT 1234
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

#include "Client.hpp"

class Server{

private:

public:
	int					listen_socket;
	struct sockaddr_in	server_address;
	struct timeval		timeout;
	int 				addr_len;
	fd_set				master_reading_set, work_reading_set, master_writing_set, work_writing_set;
	int					max_sd;
	char				buffer[BUFFER_SIZE + 1];
	int					ret, on = 1;
	bool				end_server = false;

	//std::vector<Client*> Clients;
	std::map<int, Client*> Clients;

	Server();
	~Server();

	int	start();
	int loop();
	int acceptClients();
	int receiveRequests();
	int sendResponses();

	int treatRequests();

	void disconnectClient(int socket);

};