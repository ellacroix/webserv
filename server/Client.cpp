#include "Client.hpp"

Client::~Client() {
	printf("Deleting client and closing socket\n");
	close(stream_socket);
}