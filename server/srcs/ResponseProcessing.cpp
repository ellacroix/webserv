#include "Response.hpp"

Location *		Response::findLocation(std::string & uri)
{
	size_t		longestPrefixLen;
	Location *	bestMatchLoc;
	t_locMapIt	it;
	t_locMapIt	ite;

	longestPrefixLen = 0;
	bestMatchLoc = NULL;
	it = this->virtual_server->getLocationMap().begin();
	ite = this->virtual_server->getLocationMap().end();
	while (it != ite)
	{
		if (uri.find(it->first) != std::string::npos)
		{
			if (it->first.length() > longestPrefixLen)
			{
				longestPrefixLen = it->first.length();
				bestMatchLoc = it->second;
			}
		}
		it++;
	}
	return (bestMatchLoc);
}

void			Response::constructSuccess(void)
{
	std::string	line;

	this->request_file.open(this->path.c_str());
	if (this->request_file.good() == false)
	{
		this->client->statusCode = 403;
		this->constructError();
		return ;
	}

	this->request_file.seekg (0, this->request_file.end);
	this->file_len = this->request_file.tellg();
	this->request_file.seekg (0, this->request_file.beg);

	this->raw_response.append("HTTP/1.1 200 OK\r\n");
	this->chunked = false;
	this->raw_response.append("Content-Length: ");
	this->raw_response.append(std::to_string(this->file_len));
	this->raw_response.append("\r\n");
	this->raw_response.append("Content-Type: text/html; charset=utf-8\r\n");
	this->raw_response.append("\r\n");
	while (std::getline(this->request_file, line))
		this->raw_response.append(line + '\n');
	this->request_file.close();
}

bool	Response::findIndex(void)
{
	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite;

	it = this->location->getIndex().begin();
	ite = this->location->getIndex().end();
	while (it != ite)
	{
		std::cout << "=== CHECKING " << this->path + *it << std::endl;
		if (pathExists(this->path + *it) == 1
				&& isFile(this->path + *it) == 1)
		{
			this->path.append(*it);
			std::cout << "=== FOUND INDEX "
				<< (*it) << " NEW PATH = "
				<< this->path << std::endl;
			return (true);
		}
		it++;
	}
	return (false);
}

int		Response::methodGET(void)
{
	std::cout << "=== TESTING FILE = " << this->path << std::endl;
	if (pathExists(this->path) == false)
	{
		std::cout << std::boolalpha;
		std::cout << "=== PATH DOESN'T EXIST" << std::endl;
		std::cout << "=== pathExists(this->path) == "
			<< pathExists(this->path) << std::endl;
		this->client->statusCode = 404;
		this->constructError();
		return (SUCCESS);
	}
	//	CHECK PERMISSIONS FOR METHOD
	//if (this->permissionIsOK(this->request->_method) == false)
	if (this->request->_method == "GET")
	{
		if (canRead(this->path) == false)
		{
			this->client->statusCode = 403;
			this->constructError();
			return (SUCCESS);
		}
	}
	else if (this->request->_method == "POST"
			|| this->request->_method == "DELETE")
	{
		if (canWrite(this->path) == false)
		{
			this->client->statusCode = 403;
			this->constructError();
			return (SUCCESS);
		}
	}
	if (this->path[this->path.length() -1] != '/')	//REQUEST A FILE
	{
		std::cout << "=== REQUESTING A FILE" << std::endl;
		if (isDirectory(this->path) == false)
		{
			std::cout << "=== PAGE IS FOUND" << std::endl;
			this->status_code = 200;
			//this->client->statusCode = 200;
			this->constructSuccess();
			return (SUCCESS);
		}
		else 
		{
			std::cout << "=== PAGE NOT FOUND" << std::endl;
			this->client->statusCode = 404;
			this->constructError();
			return (SUCCESS);
		}
	}
	else											//REQUEST A DIR
	{
		std::cout << "=== REQUESTING A DIRECTORY" << std::endl;
		//	CHECK index
		if (this->location->_indexIsSet == true)
		{
			std::cout << "=== CHECKING INDEXES" << std::endl;
			// findIndex() WILL REPLACE path BY A NEW PATH AND RETURN TRUE
			// IF FOUND
			if (this->findIndex() == true)
			{
				std::cout << "=== FOUND INDEX" << std::endl;
				if (canRead(this->path) == true)
				{
					this->status_code = 200;
					this->constructSuccess();
					return (SUCCESS);
				}
				else
				{
					this->client->statusCode = 403;
					this->constructError();
					return (SUCCESS);
				}
			}
			else
				std::cout << "=== NO INDEX FOUND" << std::endl;
		}
		//	CHECK autoindex
		if (this->location->_autoIndexIsSet == true
				&& this->location->getAutoIndex() == true)
		{
			this->constructAutoIndex();
			this->status_code = 200;
			this->constructSuccess();
			return (SUCCESS);
		}
		else
		{
			this->client->statusCode = 404;
			this->constructError();
			return (SUCCESS);
		}
	}
}