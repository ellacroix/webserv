#include "Response.hpp"

Response::Response(Client *parent_client)
{
	client = parent_client;
	request = parent_client->request;
	status_code = parent_client->request->_statusCode;
}

int	Response::ConstructResponse()
{
	if (client->request->status_code != 0)
		constructRequestError();
	
	//Shitty c++98 conversion of int to std::string
	std::ostringstream ss;
	ss << status_code;
	raw_response.append("RESPONSE CODE = ");
	raw_response.append(ss.str());
	raw_response.append("\n\n");
	/*
	raw_response.append("HTTP/1.1 200 OK\r\nServer: nginx/1.21.6\r\nDate: Tue, 05 Apr 2022 12:13:11 GMT\r\nContent-Type: text/plain\r\nContent-Length: 207\r\nLast-Modified: Fri, 01 Apr 2022 16:56:44 GMT\r\nConnection: keep-alive\r\nAccept-Ranges: bytes\r\n\r\n<!doctype html>\n<html lang=\"en\">\n<head>\n  <meta charset=\"utf-8\">\n  <title>Docker Nginx</title>\n</head>\n<body>\n  <h2>Hello from Nginx container</h2>\n  <h2>STATIC TESTS</h2>\n  <h2>SERVER1</h2>\n</body>\n</html>\n");
	*/




	return 0;
}
