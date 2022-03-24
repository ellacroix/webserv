#pragma once

# include "webserv.hpp"

class Virtual_Server{

private:

public:

	std::string		server_name;
	

	Virtual_Server() {};
	Virtual_Server(std::string name);
	~Virtual_Server() {};

};
