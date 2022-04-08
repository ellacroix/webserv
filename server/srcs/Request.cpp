#include "Request.hpp"

Request::Request(std::string raw, Client *parent_client) :
	_statusCode(0),
	_chunked(false),
	_parsingStep(REQUEST_LINE),
	_hasBody(false),
	_i(0),
	_nextLineI(0),
	_doubleCrlfPos(raw.length()),
	_headers(raw),
	_contentLength(-1),
	_bodyLength(-1),
	_client(parent_client),
	_virtual_server(NULL)
{
	size_t i;

	for (i = 0 ; i < N_SUPPORTED_HEADERS ; i++)
		this->_headerAlrdySet[i] = false;
	return ;
}

//	PARSING FUNCTIONS
unsigned int	Request::parser(void)
{
	printf("parser()\t- working on:\n\"%s\"\n", this->_headers.c_str());

	std::cout << "parser()\t- _doubleCrlfPos = " << this->_doubleCrlfPos << std::endl;
	while (this->_i < this->_doubleCrlfPos)
	{
		if (this->_parsingStep == REQUEST_LINE)
			this->_statusCode = this->parseReqLine();
		else if (this->_parsingStep == HEADERS)
			this->_statusCode = this->parseHeaders();
		if (this->_statusCode >= 400)
		{
			std::cout << "parser()\t- RETURNING " << this->_statusCode << std::endl;
			return (this->_statusCode);
		}
	}
	if (this->_parsingStep == BODY/* && this->_chunked == true && this->_hasBody == true*/)
		this->_statusCode = this->decodeChunk();
	std::cout << "parser()\t- EXITED LOOP" << std::endl;
	std::cout << "parser()\t- RETURNING (SUCCESS)" << std::endl;
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
	int				headerIndex;

	this->_nextLineI = this->_headers.find("\r\n", _i);
	if (this->_nextLineI == std::string::npos)
		return (400);
	headerLine = this->_headers.substr(_i, _nextLineI - _i);
	std::cout << "parseHeaders()\t- line\t= \"" << headerLine << "\"" << std::endl;
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
	std::cout << "\t\t- key\t= \"" << key << "\"" << std::endl
		<< "\t\t- value\t= \"" << value << "\"" << std::endl;
	// CHECK IF HEADER IS SUPPORTED
	headerIndex = this->isSupportedHeader(key);
	if (headerIndex != NOT_SUPPORTED_HEADER
			&& headerIndex >= HOST
			&& this->_headerAlrdySet[headerIndex] == true)
		return (400);
	if (headerIndex != NOT_SUPPORTED_HEADER)
	{
		//	CHECK VALUE
		if (this->valueIsValid(headerIndex, value) == false)
			return (400);
		this->setHeaderValue(headerIndex, value);
		this->_headerAlrdySet[headerIndex] = true;
	}

	// UPDATE CURSORS
	this->_nextLineI += 2;
	this->_i = this->_nextLineI;
	return (SUCCESS);
}

unsigned int	Request::parseReqLine(void)
{
	std::string	reqLine;
	char		*tok;
	char		*c_str;

	this->_nextLineI = this->_headers.find("\r\n");
	if (this->_nextLineI == std::string::npos)
	{
		//		std::cout << "parseReqLine()\t- NO \"\\r\\n\"" << std::endl;
		return (400);
	}
	reqLine = this->_headers.substr(_i, _nextLineI);
	std::cout << "parseReqLine()\t- reqLine = \"" << reqLine << "\"" << std::endl;

	c_str = const_cast<char*>(reqLine.c_str());

	// METHOD
	tok = std::strtok(c_str, " \t");
	//	std::cout << "parseReqLine()\t- working on tok \"" << tok << "\"" << std::endl;
	if (tok == NULL || isSupportedHttpMethod(std::string(tok)) == false)
		return (405);	//Plutot 501 "Not Implemented"
	this->_method = std::string(tok);

	//	URI
	tok = std::strtok(NULL, " \t");
	if (tok == NULL)
		return (400);
	this->_URI = std::string(tok);
	if (isValidReqUri(this->_URI) == false)
		return (400);
	//	std::cout << "parseReqLine()\t- working on tok \"" << tok << "\"" << std::endl;

	//	HTTP VERSION
	tok = std::strtok(NULL, " \t");
	if (tok == NULL)
		return (400);
	//	std::cout << "parseReqLine()\t- working on tok \"" << tok << "\"" << std::endl;
	if (tok == NULL || std::string(tok) != "HTTP/1.1")
		return (505);

	this->_nextLineI += 2;
	this->_i = this->_nextLineI;
	this->_parsingStep = HEADERS;
	std::cout << "\t\t- IS VALID" << std::endl;
	return (SUCCESS);
}

unsigned int	Request::parseBody(void)
{
	//If the method is POST
	//	If _body == NULL
	//		return (411)

	//If body is chunked
	//	return decodeChunk
	
	return (0);
}

int				Request::isSupportedHeader(std::string & key)
{
	unsigned int	i;
	size_t			len;

	len = key.length();
	for (i = 0 ; i < len ; i++)
		if (std::islower(key[i]))
			key[i] = std::tolower(key[i]);
	for (i = 0 ; i < N_SUPPORTED_HEADERS ; i++)
		if (key == Request::_supportedHeaders[i])
			return (i);
	return (NOT_SUPPORTED_HEADER);
}

const char *	Request::_supportedHeaders[N_SUPPORTED_HEADERS] =
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
