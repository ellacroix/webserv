#include "Client.hpp"

Client::Client(int fd)
{
	stream_socket = fd;
}