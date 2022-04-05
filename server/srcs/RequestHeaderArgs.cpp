#include "Request.hpp"

void	Request::setHeaderValue(const unsigned int headerIndex,
		std::string const & value)
{
	char	*endPtr;

	switch (headerIndex)
	{
		case TRANSFER_ENCODING :
			this->_chunked = true;
			this->_hasBody = true;
			break;
		case HOST :
			this->_host = value;
			break;
		case CONTENT_LENGTH :
			this->_contentLength =	std::strtol(value.c_str(), &endPtr, 10);
			this->_hasBody = true;
			break;
	}
}
