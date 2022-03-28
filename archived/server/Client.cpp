#include "Client.hpp"

Client::~Client() {
	printf("Deleting client\n");
	if (request)
		delete request;
	printf("Closing socket\n");
	close(stream_socket);
}

void	Client::createRequest(char *raw)
{
	Request *req = new Request(raw);

	if (this->request)
		delete this->request;
	this->request = req;
}