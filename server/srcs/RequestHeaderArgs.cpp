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

bool	Request::valueIsValid(const unsigned int headerIndex,
		std::string const & value)
{
	char	*endPtr;

	switch (headerIndex)
	{
		case TRANSFER_ENCODING :
			if (value == "chunked"
					&& this->_headerAlrdySet[CONTENT_LENGTH] == false
					&& this->_headerAlrdySet[TRANSFER_ENCODING] == false)
			{
				this->_chunked = true;
				this->_hasBody = true;
				return (true);
			}
			break;
		case HOST :
			if (value.find("/") == std::string::npos)
				return (true);
			break;
		case CONTENT_LENGTH :
			if (isNumber(value) == true
					&& this->_headerAlrdySet[CONTENT_LENGTH] == false
					&& this->_headerAlrdySet[TRANSFER_ENCODING] == false)
			{
				this->_contentLength =	std::strtol(value.c_str(), &endPtr, 10);
				this->_hasBody = true;
				return (true);
			}
			break;
	}
	return (false);
}
