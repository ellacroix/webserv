#include "Server.hpp"
#include <vector>

int Server::start()
{

	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket < 0){
		perror("socket() failed");
		exit(-1);
	}

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
    server_address.sin_port = htons( PORT );
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

	//Creating master_reading_set and adding listen_socket
	FD_ZERO(&master_writing_set);
	FD_ZERO(&master_reading_set);
	max_sd = listen_socket;
	FD_SET(listen_socket, &master_reading_set);

	return 0;
}

int Server::loop()
{
	int ret;

	while(end_server == false)
	{
		//Reinitializing work fd_sets before select() modified it
		work_reading_set = master_reading_set;
		work_writing_set = master_writing_set;

		//Select wait here until a socket is ready for I/O operations
		printf("\nWaiting on select()\n");
		ret = select(max_sd + 1, &work_reading_set, &work_writing_set, NULL, &timeout);
		if (ret < 0){
			perror("select() failed");
			break;
		}
		if (ret == 0){
			printf("select() timed out.  End program.\n");
			break;
		}

		acceptClients();
		sendResponses();
		receiveRequests();
		treatRequests();
	}

	for (int i = 0; i <= max_sd; ++i)
	{
		if (FD_ISSET(i, &master_reading_set))
			disconnectClient(i);
	}
	return 0;
}

int	Server::acceptClients()
{
	if (FD_ISSET(listen_socket, &work_reading_set))
	{
		int socket = 0;
		while (socket != -1)
		{
			socket = accept(listen_socket, NULL, NULL);
			if (socket < 0){
				if (errno != EWOULDBLOCK){
					perror("accept() failed");
					end_server = true;
				}
				break;
			}

			printf("Accepting new connection\n");
			Client *new_client = new Client;

			new_client->stream_socket = socket;
			Clients[new_client->stream_socket] = new_client;
			FD_SET(new_client->stream_socket, &master_reading_set);
			max_sd = new_client->stream_socket;
		}
	}

	return 0;
}

int Server::receiveRequests()
{
	Client								*ptr_c;
	std::map<int, Client*>::iterator	it_c;
	std::map<int, Client*>::iterator 	next_it;


	for (it_c = Clients.begin(), next_it = it_c; it_c != Clients.end(); it_c = next_it)
	{
		ptr_c = it_c->second;
		next_it++;
		if (FD_ISSET(ptr_c->stream_socket, &work_reading_set))
		{
			printf("Receiving data\n");
			bzero(buffer, BUFFER_SIZE);
			ret = recv(ptr_c->stream_socket, buffer, BUFFER_SIZE, 0);
			if (ret == 0)
			{
				std::cout << "Connection closed\n";
				disconnectClient(ptr_c->stream_socket);
				continue;
			}
			Clients[ptr_c->stream_socket]->createRequest(buffer);
			printf("%s\n",Clients[ptr_c->stream_socket]->request->raw_request.c_str());
			FD_SET(ptr_c->stream_socket, &master_writing_set);		//Only do this when Response is ready to be sent
		}
	}

	return 0;
}

int Server::sendResponses()
{
	int									socket;
	std::map<int, Client*>::iterator	it_c;

	for (it_c = Clients.begin(); it_c != Clients.end(); it_c++)
	{
		socket = it_c->first;
		if (FD_ISSET(socket, &work_writing_set))
		{
			printf("Sending data\n");		
			//send(socket, Clients[socket]->request->raw_request.c_str(), Clients[socket]->request->raw_request.size(), 0);
			FD_CLR(socket, &master_writing_set);
		}
	}

	return 0;
}

int Server::treatRequests()
{
	Request								*current_request;
	std::map<int, Client*>::iterator	it_c;

	for (it_c = Clients.begin(); it_c != Clients.end(); it_c++)
	{
		current_request = it_c->second->request;
		current_request->parser();
	}

	return 0;
}

void Server::disconnectClient(int socket)
{
	FD_CLR(socket, &master_reading_set);
	FD_CLR(socket, &master_writing_set);
	delete Clients[socket];
	Clients.erase(socket);
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