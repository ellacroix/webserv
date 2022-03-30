<<<<<<< HEAD:serverV2/Client.hpp
#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client {
=======
#pragma once

#include "Request.hpp"
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

class Client{
>>>>>>> main:archived/server/Client.hpp

private:

protected:

public:
	int					stream_socket;
	bool				connected = true;

	Request				*request = NULL;
	//Response			*response = NULL;
	void				createRequest(char *raw);

	Client() {};
	~Client();

<<<<<<< HEAD:serverV2/Client.hpp
};

#endif
=======
};
>>>>>>> main:archived/server/Client.hpp
