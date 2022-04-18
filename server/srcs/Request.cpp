#include "Request.hpp"

Request::Request(std::string raw, Client *parent_client) :
	_status_code(0),
	_chunked(false),
	_parsing_step(REQUEST_LINE),
	_has_body(false),
	_i(0),
	_next_lineI(0),
	_double_CRLF_pos(raw.length()),
	_headers(raw),
	_content_length(-1),
	_body_length(-1),
	_client(parent_client),
	_virtual_server(NULL)
{
	size_t i;

	for (i = 0 ; i < N_SUPPORTED_HEADERS ; i++)
		this->_header_alrdy_set[i] = false;
	return ;
}

bool			Request::isValid(void) const
{
	if (this->_host.empty() == true)
		return (false);
	return (true);
}

//	PARSING FUNCTIONS
unsigned int	Request::parser(void)
{
	//printf("parser()\t- working on:\n\"%s\"\n", this->_headers.c_str());

	//std::cout << "parser()\t- _double_CRLF_pos = " << this->_double_CRLF_pos << std::endl;
	while (this->_i < this->_double_CRLF_pos)
	{
		if (this->_parsing_step == REQUEST_LINE)
			this->_status_code = this->parseReqLine();
		else if (this->_parsing_step == HEADERS)
			this->_status_code = this->parseHeaders();
		if (this->_status_code >= 400)
		{
			//std::cout << "parser()\t- RETURNING " << this->_status_code << std::endl;
			return (this->_status_code);
		}
	}
	if (this->isValid() == false)		// CHECKS MINIMUM HEADERS
	{
		this->_status_code = 400;
		return (this->_status_code);
	}
	this->_status_code = this->parseBody();
	if (this->_status_code >= 400)
	{
		//std::cout << "parser()\t- RETURNING " << this->_status_code << std::endl;
		return (this->_status_code);
	}
	//std::cout << "parser()\t- EXITED LOOP" << std::endl;
	//std::cout << "parser()\t- RETURNING (SUCCESS)" << std::endl;
	return (SUCCESS);
}

unsigned int	Request::parseHeaders(void)
{
	std::string		headerLine;
	size_t			colonPos;
	size_t			firstPrintValChar;
	size_t			lastPrintValChar;
	std::string		key;
	std::string		value;
	int				header_index;

	this->_next_lineI = this->_headers.find("\r\n", _i);
	if (this->_next_lineI == std::string::npos)
		return (400);
	headerLine = this->_headers.substr(_i, _next_lineI - _i);
	//std::cout << "parseHeaders()\t- line\t= \"" << headerLine << "\"" << std::endl;
	// FIND ':'
	colonPos = headerLine.find(":");
	if (colonPos == std::string::npos)
		return (400);
	// SPLIT KEY:VALUE
	key = headerLine.substr(0, colonPos);
	value = headerLine.substr(colonPos + 1, headerLine.length() - colonPos + 1);
	// CHECK IF NO EMPTY VALUE
	firstPrintValChar = value.find_first_not_of(" \t");
	if (firstPrintValChar == std::string::npos)
		return (400);
	// REMOVE LEADING / TRAILING LWS
	lastPrintValChar = value.find_last_not_of(" \t");
	if (firstPrintValChar != 0 || lastPrintValChar != value.length())
		value = value.substr(firstPrintValChar,
				lastPrintValChar + 1 - firstPrintValChar);
	//std::cout << "\t\t- key\t= \"" << key << "\"" << std::endl
	//	<< "\t\t- value\t= \"" << value << "\"" << std::endl;
	// CHECK IF HEADER IS SUPPORTED
	header_index = this->isSupportedHeader(key);
	if (header_index != NOT_SUPPORTED_HEADER
			&& header_index >= HOST
			&& this->_header_alrdy_set[header_index] == true)
		return (400);
	if (header_index != NOT_SUPPORTED_HEADER)
	{
		//	CHECK VALUE
		if (this->valueIsValid(header_index, value) == false)
			return (400);
		this->setHeaderValue(header_index, value);
		this->_header_alrdy_set[header_index] = true;
	}

	// UPDATE CURSORS
	this->_next_lineI += 2;
	this->_i = this->_next_lineI;
	return (SUCCESS);
}

unsigned int	Request::parseReqLine(void)
{
	std::string	reqLine;
	char		*tok;
	char		*c_str;

	this->_next_lineI = this->_headers.find("\r\n");
	if (this->_next_lineI == std::string::npos)
	{
		//		//std::cout << "parseReqLine()\t- NO \"\\r\\n\"" << std::endl;
		return (400);
	}
	reqLine = this->_headers.substr(_i, _next_lineI);
	//printf("HERE reqline = %s\n", reqLine.c_str());
	//std::cout << "parseReqLine()\t- reqLine = \"" << reqLine << "\"" << std::endl;

	c_str = const_cast<char*>(reqLine.c_str());

	// METHOD
	tok = std::strtok(c_str, " \t");
	//	//std::cout << "parseReqLine()\t- working on tok \"" << tok << "\"" << std::endl;
	if (tok == NULL || isSupportedHttpMethod(std::string(tok)) == false)
		return (501);
	this->_method = std::string(tok);

	//	URI
	tok = std::strtok(NULL, " \t");
	if (tok == NULL)
		return (400);
	this->_URI = std::string(tok);
	if (isValidReqUri(this->_URI) == false)
		return (400);
	if (this->_URI.find("?") != std::string::npos)
		this->splitUriAndQueryString();
	//	std::cout << "parseReqLine()\t- working on tok \"" << tok << "\"" << std::endl;

	//	HTTP VERSION
	tok = std::strtok(NULL, " \t");
	if (tok == NULL)
		return (400);
	//	//std::cout << "parseReqLine()\t- working on tok \"" << tok << "\"" << std::endl;
	if (tok == NULL || std::string(tok) != "HTTP/1.1")
	{
		printf("HERE_next_lineI = %lu-%lu, reqline = %s\n", _i, _next_lineI, reqLine.c_str());
		return (505);
	}
	this->_next_lineI += 2;
	this->_i = this->_next_lineI;
	this->_parsing_step = HEADERS;
	//std::cout << "\t\t- IS VALID" << std::endl;
	return (SUCCESS);
}

unsigned int	Request::parseBody(void)
{
/* 	//De-activated until config manages clientMaxBodySize in VS
	if (_body.size() > _virtual_server->getClientMaxBodySize())
		return (413); */
	
	if (_method == "POST")
		if (_has_body == false)
			return (411);

	if (_chunked == true)
		return (decodeChunk());
	
	return (0);
}

int				Request::isSupportedHeader(std::string & key)
{
	unsigned int	i;
	size_t			len;

	len = key.length();
	for (i = 0 ; i < len ; i++)
		if (std::isupper(key[i]))
			key[i] = std::tolower(key[i]);
	for (i = 0 ; i < N_SUPPORTED_HEADERS ; i++)
		if (key == Request::_supported_headers[i])
			return (i);
	return (NOT_SUPPORTED_HEADER);
}

const char *	Request::_supported_headers[N_SUPPORTED_HEADERS] =
{
	"transfer-encoding", "host", "content-length"
};


unsigned int	Request::decodeChunk(void)
{	
	std::string decodedBody;

	size_t		chunkSize = strtol(_body.c_str(), NULL, 16);
	size_t		chunkStart = 0;
	std::string chunk;

	while (chunkSize > 0)
	{
		chunkStart = _body.find("\r\n", chunkStart) + 2;
		chunk = _body.substr(chunkStart, chunkSize);
		printf("Chunk %lu = %s\n", chunkSize, chunk.c_str());
		decodedBody.append(chunk.c_str(), chunkSize);
		chunkStart += chunkSize + 2;
		chunkSize = strtol(_body.c_str() + chunkStart, NULL, 16);
	}

	//Checking if we're on 0 and the chunked is not corrupted
	size_t		bodyEnd = _body.find("\r\n0\r\n") + 2;
	if (chunkStart != bodyEnd)
		return 400;

	this->_body = decodedBody;
	return 0;
}

VirtualServer *	Request::findVirtualServer(std::string & s)
{
	t_VS_listCIt							it;
	t_VS_listCIt							ite;
	std::vector<std::string>::iterator	serverNameEnd;

	it = this->_client->parent_port->_VS_list.begin();
	ite = this->_client->parent_port->_VS_list.end();
	while (it != ite)
	{
		serverNameEnd = (*it)->getServerName().end();
		if (std::find((*it)->getServerName().begin(),
					(*it)->getServerName().end(), s) != serverNameEnd)
			return (*it);
		it++;
	}
	return (this->_client->parent_port->_VS_list.front());
}

void			Request::splitUriAndQueryString(void)
{
	size_t	question_mark_pos;
	
	question_mark_pos = this->_URI.find("?");
	this->_query_string = this->_URI.substr(question_mark_pos + 1,
			this->_URI.length() - question_mark_pos);
	this->_URI = this->_URI.substr(0, question_mark_pos);
}
