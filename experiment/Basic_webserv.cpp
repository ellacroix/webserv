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

#define PORT 1234
#define BUFFER_SIZE 50

//Basic server for only one connection

int main() {
	int					listen_socket, client_socket;
	struct sockaddr_in	server_address, client_address;
	fd_set				master_reading_set, work_reading_set, master_writing_set, work_writing_set;
	struct timeval		timeout;
	int					max_sd;
	char				buffer[BUFFER_SIZE + 1];
	int					ret, on = 1;
	int 				addr_len = sizeof(client_address);


	listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	//Prevent “Address already in use" error by the OS
	setsockopt(listen_socket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));

	//Basic binc
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( PORT );
	bind(listen_socket, (struct sockaddr *)&server_address, sizeof(server_address));

	//Set O_NONBLOCK flag on the listening socket, so it transmits to sockets created with accept()
	fcntl(listen_socket, F_SETFL, O_NONBLOCK);

	listen(listen_socket, 1);
	
	//Creating master_reading_set and adding listen_socket
	FD_ZERO(&master_writing_set);
	FD_ZERO(&master_reading_set);
	max_sd = listen_socket;
	FD_SET(listen_socket, &master_reading_set);

	//Setting timeout params for select()
	timeout.tv_sec  = 60;
	timeout.tv_usec = 0;

	while(1)
	{
		//Reinitializing work fd_sets before select() modified it
		work_reading_set = master_reading_set;
		work_writing_set = master_writing_set;

		//Select wait here until a socket is ready for I/O operations
		select(max_sd + 1, &work_reading_set, &work_writing_set, NULL, NULL);

		//Check if the listening socket has some connections to accept()
		if (FD_ISSET(listen_socket, &work_reading_set))
		{
			client_socket = accept(listen_socket, (struct sockaddr *)&client_address, (socklen_t*)&addr_len);
			FD_SET(client_socket, &master_reading_set);
			max_sd = client_socket;
		}

		//Check if we added the client_socket to writing_set and if it is ready to be written to
		if (FD_ISSET(client_socket, &work_writing_set))
		{
			send(client_socket, buffer, strlen(buffer), 0);
			FD_CLR(client_socket, &master_writing_set);
			max_sd = listen_socket;
		}

		//Check if the client_socket has something to be read in it
		if (FD_ISSET(client_socket, &work_reading_set))
		{
			ret = recv(client_socket, buffer, BUFFER_SIZE, 0);
			buffer[ret] = 0;
			FD_SET(client_socket, &master_writing_set);
		}
	}

}