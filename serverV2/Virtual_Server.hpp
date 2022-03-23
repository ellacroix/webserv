#pragma once

#include <vector>
#include <map>
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

class Virtual_Server{

private:

public:

	std::string		server_name;

	Virtual_Server() {};
	Virtual_Server(std::string name);
	~Virtual_Server() {};

};