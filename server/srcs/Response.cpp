#include "Response.hpp"

Response::Response(Client *parent_client)
{
	client = parent_client;
	request = parent_client->request;
	status_code = parent_client->request->_statusCode;
}

int	Response::ConstructResponse()
{
	//Shitty c++98 conversion of int to std::string
	std::ostringstream ss;
	ss << status_code;
	raw_response = ss.str();

	return 0;
}
