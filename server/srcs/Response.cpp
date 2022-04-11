#include "Response.hpp"

std::string numberToString(size_t nb)
{
	std::ostringstream oss;

	oss << nb;
	return oss.str();
}

Response::Response(Client *parent_client)
{
	client = parent_client;
	request = parent_client->request;
	//status_code = parent_client->request->_statusCode;
	status_code = parent_client->statusCode;
	//	chunked = false;
}

int	Response::ConstructResponse()
{	
	//	FIND server
	if (this->request == NULL)
		this->virtual_server = client->parent_port->_VSList.front();
	else
		this->virtual_server = this->findVirtualServer(this->request->_host);
	if (client->statusCode != 0)
	{
		//	IF ERROR DETECTED IN PARSING
		constructError();
		return (SUCCESS);
	}
	//	ELSE (PROCESSING CONTINUES)
	//	constructError();
	//	FIND location
	this->location = this->findLocation(this->request->_URI);
	if (this->location == NULL)
	{
		this->client->statusCode = 404;
		this->constructError();
		return (SUCCESS);
	}
	std::cout << "=== FOUND LOCATION = "
		<< this->location->getPrefix() << std::endl;
	//	CHECK limit_except
	if (this->location->_limitExceptIsSet == true &&
			std::find(this->location->getLimitExcept().begin(),
				this->location->getLimitExcept().end(),
				this->request->_method)
			!= this->location->getLimitExcept().end())
	{
		this->client->statusCode = 405;
		this->constructError();
		return (SUCCESS);
	}
	/*
	//	CHECK return
	if (this->location->_returnIsSet == true)
	{
	this->client->statusCode = this->location->getReturnCode();
	this->location_header = this->location->getReturnUri();
	this->constructRedirection();
	return (SUCCESS);
	}
	*/
	//	MAKE DIR/FILE PATH	- ROOT VERSION
	//this->path = this->location.getRoot().append(this->request->_URI);
	//						- ALIAS VERSION
	this->path = this->request->_URI;
	this->path.replace(0, this->location->getPrefix().length(),
			this->location->getRoot());
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
				&& this->location.getAutoIndex() == true)
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
	this->client->statusCode = 404;
	this->constructError();
	return (SUCCESS);
}

void	Response::constructError()
{
	//if we don't find _statusCode in a std::map<code, File>, we send the default error
	/* 	if (virtual_server->getErrorPage().find(client->statusCode) != virtual_server->getErrorPage().end())
		{
		printf("Page defined in config file\n");
		std::string path = virtual_server->getErrorPage().find(client->statusCode)->second;

		if (pathExists(path) && isFile(path) && canRead(path))
		{
		std::ifstream file(path.c_str() + 1);
		std::stringstream buffer;
		buffer << file.rdbuf();
		body.append(buffer.str());
		}
		else
		printf("Page not found\n");
		} */

	//Constructing body
	std::string		body;

	if (body.empty())
	{
		printf("Redacting default page\n");
		body.append("<html>\r\n");
		body.append("<head><title>" + numberToString(client->statusCode) + getErrorMessage(client->statusCode) + "</title></head>\r\n");
		body.append("<body>\r\n");
		body.append("<center><h1> DEFAULT PAGE " + numberToString(client->statusCode) + getErrorMessage(client->statusCode) + "</h1></center>\r\n");
		body.append("</body>\r\n");
		body.append("</html>\r\n");
	}

	//Status line
	raw_response.append("HTTP/1.1 ");
	raw_response.append(numberToString(client->statusCode));
	raw_response.append(getErrorMessage(client->statusCode));
	//Additional Info ??
	raw_response.append("\r\n");

	//Headers
	raw_response.append("Content-Length: " + numberToString(body.size()) + "\r\n");
	raw_response.append("Content-Type: text/html; charset=UTF-8\r\n");
	raw_response.append("\r\n");

	//Body
	raw_response.append(body);
}

std::string	Response::getErrorMessage(int code)
{
	switch (code)
	{
		case 200:
			return " OK";
		case 201:
			return " Created";
		case 204:
			return " No Content";
		case 206:
			return " Partial Content";
		case 301:
			return " Moved Permanently";
		case 304:
			return " Not Modified";
		case 400:
			return " Bad Request";
		case 401:
			return " Unauthorized";
		case 403:
			return " Forbidden";
		case 404:
			return " Not Found";
		case 405:
			return " Method Not Allowed";
		case 406:
			return " Not Acceptable";
		case 408:
			return " Request Timeout";
		case 411:
			return " Length Required";
		case 413:
			return " Request Entity Too Large";
		case 414:
			return " Request-URI Too Long";
		case 415:
			return " Unsupported Media Type";
		case 417:
			return " Expectation Failed";
		case 431:
			return " Request Header Fields Too Large";
		case 500:
			return " Internal Server Error";
		case 501:
			return " Not Implemented";
		case 505:
			return " HTTP Version Not Supported";			
		default:
			return " Should not happen";
	}
}
