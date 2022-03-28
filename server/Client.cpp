#include "Client.hpp"

Client::Client(int fd) :
	connected(true)
{
	stream_socket = fd;
	pthread_mutex_init(&client_mutex, NULL);

	response = false;
}
