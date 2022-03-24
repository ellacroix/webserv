#include "Client.hpp"

Client::Client(int fd) :
	connected(true)
{
	stream_socket = fd;
}
