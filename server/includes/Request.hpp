#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

# include "Client.hpp"
# include "VirtualServer.hpp"

enum    e_parsingSteps
{
	REQ_ERROR = -1,
	REQUEST_LINE,
	HEADERS,
	BODY
};

# define N_SUPPORTED_HEADERS 3


enum	e_supportedHeaders
{
	NOT_SUPPORTED_HEADER = -1,
	TRANSFER_ENCODING,
	// HEADERS THAT CAN BE PRESENT ONLY ONCE START AT HOST
	HOST,	
	CONTENT_LENGTH
};

class Client ;
class VirtualServer ;

class   Request {
	private:
		static const char *	_supportedHeaders[N_SUPPORTED_HEADERS];

	public:
		//  HEADERS PARSING
		unsigned int		_statusCode;
		bool				_chunked;
		int					_parsingStep;
		std::string			_curLine;
		bool				_hasBody;
		//	PARSING CURSORS
		size_t				_i;
		size_t				_nextLineI;
		size_t				_doubleCrlfPos;

		//  REQUEST LINE
		std::string			_method;
		std::string			_URI;
		//  HEADERS
		std::string			_headers;
		ssize_t				_contentLength;
		std::string			_host;
		std::string			_transferEncoding;
		bool				_headerAlrdySet[N_SUPPORTED_HEADERS];
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
		//		Request.cpp
		unsigned int	parser(void);
		unsigned int	parseReqLine(void);
		unsigned int	parseHeaders(void);
		unsigned int	parseBody(void);
		int				isSupportedHeader(std::string & key);
		//		RequestHeaderArgs.cpp
		void	setHeaderValue(const unsigned int headerIndex,
				std::string const & value);

};


#endif
