#include "Request.hpp"

//Simple tester to test a single HTTP request in av[1]
int main(int ac, char **av)
{
	(void)ac;
	Request *request = new Request();
	char buffer[5000];

	int fd = open(av[1], O_RDONLY);
	read(fd, buffer, 5000);

	request->raw_request = buffer;
	printf("Request: %s\n", request->raw_request.c_str());

	request->parser();
}