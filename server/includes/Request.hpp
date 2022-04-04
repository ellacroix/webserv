#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "webserv.hpp"

#include "Client.hpp"
#include "VirtualServer.hpp"

class Client ;
class VirtualServer ;

enum    parsingSteps
{
	REQ_ERROR = -1,
	REQUEST_LINE,
	HEADERS,
	BODY
};

class   Request {
	private:

	public:
		//  PARSING
		unsigned int		_statusCode;
		bool				_chunked;
		int					_parsingStep;
		std::string			_curLine;
		size_t				_i;
		size_t				_nextLineI;
		size_t				_doubleCrlfPos;
		bool				_hasBody;

		//  REQUEST LINE
		std::string			_method;
		std::string			_URI;
		//  HEADERS
		std::string			_headers;
		ssize_t				_contentLength;
		std::string			_host;
		std::string			_transferEncoding;
		//  BODY
		std::string			_body;
		ssize_t				_bodyLength;

		//	CLASSES REQ DEPENDS ON
		Client				*_client;
		VirtualServer       *_virtual_server;

		Request(void) {};
		~Request(void) {};
		Request(std::string raw, Client *parent_client);

		//	PARSING FUNCTIONS
		unsigned int	parser(void);
		unsigned int	parseReqLine(void);
		unsigned int	parseHeaders(void);
		unsigned int	parseBody(void);
		unsigned int	isSupportedHeader(void);
};


#endif
