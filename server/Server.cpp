#include "Server.hpp"
#include <vector>

int Server::start()
{

	listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	//Prevent “Address already in use" error by the OS
	setsockopt(listen_socket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));

	//Basic bind
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( PORT );
	addr_len = sizeof(server_address);
	bind(listen_socket, (struct sockaddr *)&server_address, addr_len);

	//Set O_NONBLOCK flag on the listening socket, so it transmits to sockets created with accept()
	fcntl(listen_socket, F_SETFL, O_NONBLOCK);

	listen(listen_socket, 1);
	
	//Creating master_reading_set and adding listen_socket
	FD_ZERO(&master_writing_set);
	FD_ZERO(&master_reading_set);
	max_sd = listen_socket;
	FD_SET(listen_socket, &master_reading_set);

	return 0;
}

int Server::loop()
{
	Client							*ptr_c;
	std::vector<Client*>::iterator	it_c;

	while(1)
	{
		//Reinitializing work fd_sets before select() modified it
		work_reading_set = master_reading_set;
		work_writing_set = master_writing_set;

		//Select wait here until a socket is ready for I/O operations
		std::cout << "\nWaiting for select()\n";
		select(max_sd + 1, &work_reading_set, &work_writing_set, NULL, &timeout);

		acceptClients();

		sendResponses();

		receiveRequests();
	}
	return 0;
}

int	Server::acceptClients()
{
	if (FD_ISSET(listen_socket, &work_reading_set))
	{
		std::cout << "Accepting new connection\n";		
		Client *new_client = new Client;

		new_client->stream_socket = accept(listen_socket, (struct sockaddr *)&new_client->client_address, (socklen_t*)&addr_len);
		Clients.push_back(new_client);
		FD_SET(new_client->stream_socket, &master_reading_set);
		max_sd = new_client->stream_socket;
	}

	return 0;
}

int Server::receiveRequests()
{
	Client							*ptr_c;
	std::vector<Client*>::iterator	it_c;

	for (it_c = Clients.begin(); it_c != Clients.end(); it_c++)
	{
		ptr_c = *it_c;
		if (FD_ISSET(ptr_c->stream_socket, &work_reading_set))
		{
			std::cout << "Receiving data\n";		
			ret = recv(ptr_c->stream_socket, buffer, BUFFER_SIZE, 0);
			buffer[ret] = 0;
			FD_SET(ptr_c->stream_socket, &master_writing_set);
		}
	}

	return 0;
}

int Server::sendResponses()
{
	Client							*ptr_c;
	std::vector<Client*>::iterator	it_c;
	
	for (it_c = Clients.begin(); it_c != Clients.end(); it_c++)
	{
		ptr_c = *it_c;
		if (FD_ISSET(ptr_c->stream_socket, &work_writing_set))
		{
			std::cout << "Sending data\n";		
			send(ptr_c->stream_socket, buffer, strlen(buffer), 0);
			FD_CLR(ptr_c->stream_socket, &master_writing_set);
		}
	}

	return 0;
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