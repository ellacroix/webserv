#include "Client.hpp"

Client::Client(int fd, Port *port) :
	connected(true)
{
	parent_port = port;
	stream_socket = fd;
	pthread_mutex_init(&client_mutex, NULL);

	response_ready = false;
}

Client::~Client() {
	printf("Deleting client\n");
 	if (request)
		delete request;
	pthread_mutex_destroy(&client_mutex);
	printf("Closing socket\n");
	close(stream_socket);
}

bool	Client::CheckCompleteRequest()
{
	size_t pos = request_buffer.find("\r\n\r\n");
	if (pos != std::string::npos)
		return true;
	return false;
}

int	Client::CreateRequest()
{
	Request *req = new Request(request_buffer);

	if (this->request)
		delete this->request;
	this->request = req;

	return 0;
}