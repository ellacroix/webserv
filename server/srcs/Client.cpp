#include "Client.hpp"

Client::Client(int fd, Port *port) :
	connected(true)
{
	parent_port = port;
	stream_socket = fd;
	pthread_mutex_init(&client_mutex, NULL);
	gettimeofday(&last_activity, NULL);
	request = NULL;
	response = NULL;

	response_ready = false;
	read_more = false;
	suicide = false;
}

Client::Client()
{
	stream_socket = -1;
	request = NULL;
	response = NULL;
	pthread_mutex_init(&client_mutex, NULL);
	suicide = true;
}

Client::~Client() {
	printf("\033[0;31m");
	printf("MainProcess: Deleting client %d\n", stream_socket);
 	if (request)
		delete request;
	printf("MainProcess: Destroying mutex %p\n", &client_mutex);
	pthread_mutex_destroy(&client_mutex);
	printf("MainProcess: Closing socket %d\n", stream_socket);
	if (stream_socket != -1)
		close(stream_socket);
	printf("\033[0m");
}

int	Client::createRequest()
{
	std::string request_headers = request_buffer.substr(0, request_buffer.find("\r\n\r\n") + 2);

	Request *req = new Request(request_headers, this);

	if (this->request)
		delete this->request;
	this->request = req;

	return 0;
}

int	Client::createResponse()
{
	Response *resp = new Response(this);

	if (this->response)
		delete this->response;
	this->response = resp;

	return 0;
}
