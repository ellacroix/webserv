#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

# include "Client.hpp"
# include "VirtualServer.hpp"

enum    e_parsing_steps
{
	REQ_ERROR = -1,
	REQUEST_LINE,
	HEADERS,
	BODY,
	COMPLETE
};

# define N_SUPPORTED_HEADERS 3


enum	e_supported_headers
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
		static const char *	_supported_headers[N_SUPPORTED_HEADERS];

	public:
		//  HEADERS PARSING
		unsigned int		_status_code;
		bool				_chunked;
		int					_parsing_step;
		std::string			_cur_line;
		bool				_has_body;
		//	PARSING CURSORS
		size_t				_i;
		size_t				_next_lineI;
		size_t				_double_CRLF_pos;

		//  REQUEST LINE
		std::string			_method;
		std::string			_URI;
		std::string			_query_string;

		//  HEADERS
		std::string			_headers;
		ssize_t				_content_length;
		std::string			_host;
		std::string			_transfer_encoding;
		bool				_header_alrdy_set[N_SUPPORTED_HEADERS];
		//  BODY
		std::string			_body;
		ssize_t				_body_length;

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
		void			splitUriAndQueryString(void);
		int             isSupportedHeader(std::string & key);

		//		RequestHeaderArgs.cpp
		void			setHeaderValue(const unsigned int header_index,
						std::string const & value);
		bool			valueIsValid(const unsigned int header_index,
						std::string const & value);

		unsigned int	decodeChunk(void);
		VirtualServer	*findVirtualServer(std::string & s);
		bool			isValid(void) const;
};


#endif
