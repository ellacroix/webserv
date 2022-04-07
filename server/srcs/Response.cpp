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
	
	//Shitty c++98 conversion of int to std::string
	std::ostringstream ss;\
	ss << status_code;
/* 	raw_response.append("RESPONSE CODE = ");
	raw_response.append(ss.str());
	raw_response.append("\n\n"); */
	/*
	raw_response.append("HTTP/1.1 200 OK\r\nServer: nginx/1.21.6\r\nDate: Tue, 05 Apr 2022 12:13:11 GMT\r\nContent-Type: text/plain\r\nContent-Length: 207\r\nLast-Modified: Fri, 01 Apr 2022 16:56:44 GMT\r\nConnection: keep-alive\r\nAccept-Ranges: bytes\r\n\r\n<!doctype html>\n<html lang=\"en\">\n<head>\n  <meta charset=\"utf-8\">\n  <title>Docker Nginx</title>\n</head>\n<body>\n  <h2>Hello from Nginx container</h2>\n  <h2>STATIC TESTS</h2>\n  <h2>SERVER1</h2>\n</body>\n</html>\n");
	*/

	if (client->statusCode != 0)
		constructError();


	return 0;
}

void	Response::constructError()
{
	//if we don't find _statusCode in a std::map<code, File>, we send the default error
/* 	if (virtual_server->_errorPage.find(client->statusCode) != virtual_server->_errorPage.end())
	{
		printf("Page defined in config file\n");
	} */

	printf("Sending default page\n");

	//Constructing body
	body.append("<html>\r\n");
	body.append("<head><title>" + numberToString(client->statusCode) + " Corresponding message</title></head>\r\n");
	body.append("<body>\r\n");
	body.append("<center><h1>" + numberToString(client->statusCode) + " Corresponding message</h1></center>\r\n");
	body.append("</body>\r\n");
	body.append("</html>\r\n");

	//Status line
	raw_response.append("HTTP/1.1 ");
	raw_response.append(numberToString(client->statusCode));
	raw_response.append(" Bad Request");
	//Additional Info ??
	raw_response.append("\r\n");

	//Headers
	raw_response.append("Content-Length: " + numberToString(body.size()) + "\r\n");
	raw_response.append("Content-Type: text/html; charset=UTF-8\r\n");
	raw_response.append("\r\n");

	//Body
	raw_response.append(body);


	
}
