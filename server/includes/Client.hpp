#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Port.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include "webserv.hpp"

class Port ;
class Request ;
class Response ;

class Client{

	private:

	protected:

	public:
		Port				*parent_port;
		int					stream_socket;
		bool				connected;
		std::string			request_buffer;
		pthread_mutex_t		client_mutex;
		struct timeval		last_activity;

		bool				read_more;
		bool				response_ready;

		Request				*request;
		Response			*response;

		Client() {};
		Client(int fd, Port *port);
		~Client();

		int					CreateRequest();
		int					CreateResponse();

};

#endif
