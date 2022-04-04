#include "Request.hpp"

Request::Request(std::string raw, Client *parent_client) :
	_statusCode(0),
	_chunked(false),
	_parsingStep(REQUEST_LINE),
	_i(0),
	_nextLineI(0),
	_doubleCrlfPos(raw.length()),
	_hasBody(false),
	_headers(raw),
	_contentLength(-1),
	_bodyLength(-1),
	_client(parent_client),
	_virtual_server(NULL)
{
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
		if (this->_parsingStep == HEADERS)
			this->_statusCode = this->parseHeaders();
		/*
		if (this->_parsingStep == BODY)
			this->_statusCode = this->parseBody();
		*/
		if (this->_statusCode >= 400)
		{
			std::cout << "parser()\t- RETURNING " << this->_statusCode << std::endl;
			return (this->_statusCode);
		}
	}
	std::cout << "parser()\t- EXITED LOOP" << std::endl;
	return SUCCESS;
}

unsigned int	Request::parseHeaders(void)
{
	std::string	headerLine;
	size_t		colonPos;
	size_t		firstPrintValChar;
	std::string	key;
	std::string	value;

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
	firstPrintValChar = value.find_first_not_of(" \t");
	// CHECK IF OTHER ':' OR IF NO EMPTY VALUE
	if (value.find(":") != std::string::npos
			|| firstPrintValChar == std::string::npos)
		return (400);
	// REMOVE HEADING LWS
	else if (firstPrintValChar != 0)
		value = value.substr(firstPrintValChar, value.length() - firstPrintValChar);
	std::cout << "\t\t- key\t= \"" << key << "\"" << std::endl
		<< "\t\t- value\t= \"" << value << "\"" << std::endl;

	//	CHECK IF HEADER IS SUPPORTED
	//	CHECK VALUE

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
		return (405);
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
	return (0);
}
