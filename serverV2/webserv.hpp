#ifndef WEBSERV_HPP
# define WEBSERV_HPP

//	GLIBC
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

//	CPP
#include <vector>
#include <map>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

//	CUSTOM HEADERS
/*
#include "Location.hpp"
#include "VirtualServer.hpp"
#include "Client.hpp"
#include "Port.hpp"
#include "ConfigParser.hpp"
*/

//	utils.cpp
bool			isNumber(std::string s);
bool			isValidPrefix(std::string *s);
bool			isValidPort(std::string s);
bool			isValidDomainName(std::string s);
bool			isValidClientMaxBodySize(std::string s);
bool			isValidHttpErrorCode(std::string s);
bool			isValidUri(std::string & s);
bool			isValidErrorPage(std::vector<std::string> & v);
bool			isValidAutoindex(std::string s);
//bool			isValidIndex(std::vector<std::string> v);
bool			isValidIndex(std::string s);
bool			isSupportedHttpRedirCode(std::string s);
bool			isValidReturn(std::vector<std::string> & v);
bool			isSupportedHttpMethod(std::string s);
bool			isValidLimitExcept(std::vector<std::string> v);

#endif
