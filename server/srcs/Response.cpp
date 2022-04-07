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
}

int	Response::ConstructResponse()
{	
	// if this->_request == NULL
	//		this->_virtualServer = client->parent_port->_VSList.front();
	if (client->statusCode != 0)
		constructError();
	
	//else Response a du travail a faire pour determiner le code de reponse


	return 0;
}

void	Response::constructError()
{
/* 	//if we don't find _statusCode in a std::map<code, File>, we send the default error
	if (virtual_server->getErrorPage().find(client->statusCode) != virtual_server->getErrorPage().end())
	{
		printf("Page defined in config file\n");
	} */

	printf("Sending default page\n");

	//Constructing body
	body.append("<html>\r\n");
	body.append("<head><title>" + numberToString(client->statusCode) + getErrorMessage(client->statusCode) + "</title></head>\r\n");
	body.append("<body>\r\n");
	body.append("<center><h1> DEFAULT PAGE " + numberToString(client->statusCode) + getErrorMessage(client->statusCode) + "</h1></center>\r\n");
	body.append("</body>\r\n");
	body.append("</html>\r\n");

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
			return " Forbidden";			
		default:
			return "Should not happen";
	}
}
