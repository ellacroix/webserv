#include "Request.hpp"

void	Request::setHeaderValue(const unsigned int header_index,
		std::string const & value)
{

	switch (header_index)
	{
		case TRANSFER_ENCODING :
			this->_chunked = true;
			this->_has_body = true;
			break;
		case HOST :
			this->_host = value;
			this->_virtual_server = findVirtualServer(_host);
			break;
		case CONTENT_LENGTH :
			this->_content_length = std::strtol(value.c_str(), NULL, 10);
			this->_has_body = true;
			break;
	}
}

bool	Request::valueIsValid(const unsigned int header_index,
		std::string const & value)
{
	char	*endPtr;

	//std::cout << "HEADER_INDEX = " << header_index << std::endl;

	switch (header_index)
	{
		case TRANSFER_ENCODING :
			if (value == "chunked"
					&& this->_header_alrdy_set[CONTENT_LENGTH] == false
					&& this->_header_alrdy_set[TRANSFER_ENCODING] == false)
			{
				this->_chunked = true;
				this->_has_body = true;
				return (true);
			}
			break;
		case HOST :
			if (value.find("/") == std::string::npos)
				return (true);
			break;
		case CONTENT_LENGTH :
			if (isNumber(value) == true
					&& this->_header_alrdy_set[CONTENT_LENGTH] == false
					&& this->_header_alrdy_set[TRANSFER_ENCODING] == false)
			{
				this->_content_length =	std::strtol(value.c_str(), &endPtr, 10);
				this->_has_body = true;
				return (true);
			}
			break;
		case CONTENT_TYPE :
			if (this->_header_alrdy_set[CONTENT_TYPE] == false)
			{
				this->_header_alrdy_set[CONTENT_TYPE] = true;
				this->_content_type = value;
				return (true);
			}
			break;
	}
	std::cout << "VALUE IS false" << std::endl;
	return (false);
}
