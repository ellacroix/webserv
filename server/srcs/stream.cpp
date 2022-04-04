#include <stdio.h>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>



int main()
{
	std::string string("Hello there\n");
	std::istringstream stream(string);
	char	buf[500];

	bzero(buf, 500);
	stream.get(buf, 4);
	std::cout << buf << std::endl;
	stream.get(buf, 4);
	std::cout << buf << std::endl;
	stream.get(buf, 4);
	std::cout << buf << std::endl;
	stream.get(buf, 4);
	std::cout << buf << std::endl;
	stream.get(buf, 4);
	std::cout << buf << std::endl;
}