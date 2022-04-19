#include "Response.hpp"

void            Response::construct200Ok(void)
{
    std::string line;
	std::string	extension;
	char		*buffer;

	//std::cout << "=== CONSTRUCT SUCCESS RESPONSE" << std::endl;
	if (!this->isCgi(this->path))
	{
		this->request_file.open(this->path.c_str());
		if (this->request_file.good() == false)
		{
			this->client->status_code = 403;
			this->constructError();
			return ;
		}

		//	FILE SIZE
		this->request_file.seekg (0, this->request_file.end);
		this->file_len = this->request_file.tellg();
		this->request_file.seekg (0, this->request_file.beg);
	}
	else
		this->file_len = body.size();

	//	STATUS LINE
    this->raw_response.append("HTTP/1.1 ");
	this->raw_response.append(numberToString(this->client->status_code));
	this->raw_response.append(getErrorMessage(this->client->status_code) + "\r\n");
	//	HEADERS
    this->raw_response.append("Content-Length: ");
	this->raw_response.append(numberToString(this->file_len) + "\r\n");
	//printf("THIS is CGI ? %d\n", this->isCgi(this->path));
	if (!this->isCgi(this->path))
	{
		this->raw_response.append("Content-Type: ");
		this->raw_response.append(this->findContentType() + "\r\n");
		this->raw_response.append("\r\n");
	}

	if (!this->isCgi(this->path))
	{
		buffer = new char[this->file_len];
		this->request_file.read(buffer, this->file_len);

	//    while (std::getline(this->request_file, line))
	//	    this->raw_response.append(line + '\n');
		this->raw_response.append(buffer, this->file_len);
		
		delete [] buffer;
		this->request_file.close();
	}
	this->raw_response.append(body);
}

void    Response::constructRedirection()
{
    std::string redir_URI = location->getReturnUri() + request->_URI.substr(location->getPrefix().length() - 1);
    
    if (body.empty())
	{
		//printf("Redacting default page\n");
		body.append("<html>\r\n");
		body.append("<head><title>" + numberToString(client->status_code) + getErrorMessage(client->status_code) + "</title></head>\r\n");
		body.append("<body>\r\n");
		body.append("<center><h1> DEFAULT PAGE " + numberToString(client->status_code) + getErrorMessage(client->status_code) + "</h1></center>\r\n");
		body.append("<center><a href=\"" + redir_URI +"\"> TRY HERE </a></center>");
        body.append("</body>\r\n");
		body.append("</html>\r\n");
	}

    //Status line
	raw_response.append("HTTP/1.1 ");
	raw_response.append(numberToString(client->status_code));
	raw_response.append(getErrorMessage(client->status_code));
	//Additional Info ??
	raw_response.append("\r\n");

	//Headers
	raw_response.append("Content-Length: " + numberToString(body.size()) + "\r\n");
	raw_response.append("Content-Type: text/html; charset=UTF-8\r\n");
	raw_response.append("Location: " + redir_URI + "\r\n");
	raw_response.append("\r\n");

	//Body
	raw_response.append(body);
    
    return ;
}

void	Response::constructError()
{
 	//if we don't find _status_code in a std::map<code, File>, we send the default error
	if (body.empty())
	{
		//printf("Redacting default page\n");
		body.append("<html>\r\n");
		body.append("<head><title>" + numberToString(client->status_code) + getErrorMessage(client->status_code) + "</title></head>\r\n");
		body.append("<body>\r\n");
		body.append("<center><h1> DEFAULT PAGE " + numberToString(client->status_code) + getErrorMessage(client->status_code) + "</h1></center>\r\n");
		body.append("</body>\r\n");
		body.append("</html>\r\n");
	}

	//Status line
	raw_response.append("HTTP/1.1 ");
	raw_response.append(numberToString(client->status_code));
	raw_response.append(getErrorMessage(client->status_code));
	//Additional Info ??
	raw_response.append("\r\n");

	//Headers
	raw_response.append("Content-Length: " + numberToString(body.size()) + "\r\n");
	raw_response.append("Content-Type: text/html; charset=UTF-8\r\n");
	raw_response.append("\r\n");

	//Body
	raw_response.append(body);
}

std::string	Response::getErrorMessage(int code)
{
	switch (code)
	{
		case 200:
			return " OK";
		case 201:
			return " Created";
		case 204:
			return " No Content";
		case 206:
			return " Partial Content";
		case 301:
			return " Moved Permanently";
		case 302:
			return " Moved Temporarily";
		case 304:
			return " Not Modified";
		case 400:
			return " Bad Request";
		case 401:
			return " Unauthorized";
		case 403:
			return " Forbidden";
		case 404:
			return " Not Found";
		case 405:
			return " Method Not Allowed";
		case 406:
			return " Not Acceptable";
		case 408:
			return " Request Timeout";
		case 411:
			return " Length Required";
		case 413:
			return " Request Entity Too Large";
		case 414:
			return " Request-URI Too Long";
		case 415:
			return " Unsupported Media Type";
		case 417:
			return " Expectation Failed";
		case 431:
			return " Request Header Fields Too Large";
		case 500:
			return " Internal Server Error";
		case 501:
			return " Not Implemented";
		case 505:
			return " HTTP Version Not Supported";			
		default:
			return " Should not happen";
	}
}

int		Response::createFile(void)
{
	std::cout << "=== POST - OPENING " << this->path << std::endl;
	this->post_file.open(this->path.c_str(),
			std::ostream::out | std::ostream::trunc);
	if (this->post_file.good() == false)
	{
		this->client->status_code = 403;
		this->constructError();
		return 403;
	}
	std::cout << "=== POST - OPENED " << this->path << std::endl;
	this->post_file << this->request->_body;
	this->post_file.close();
	return (SUCCESS);
}

void	Response::construct201Created(void)
{
    if (body.empty())
	{
		body.append("<html>\r\n");
		body.append("<head><title>" + numberToString(client->status_code) + getErrorMessage(client->status_code) + "</title></head>\r\n");
		body.append("<body>\r\n");
		body.append("<center><h1> Your file was succesfully created</h1></center>\r\n");
		body.append("<center><h1> retrieve it <a href=\""
				+ this->request->_URI
				+ "\"> HERE </a></center>");
        body.append("</body>\r\n");
		body.append("</html>\r\n");
	}

    //Status line
	raw_response.append("HTTP/1.1 ");
	raw_response.append(numberToString(client->status_code));
	raw_response.append(getErrorMessage(client->status_code));
	//Additional Info ??
	raw_response.append("\r\n");

	//Headers
	raw_response.append("Content-Length: " + numberToString(body.size()) + "\r\n");
	raw_response.append("Content-Type: text/html; charset=UTF-8\r\n");
	raw_response.append("Location: " + this->request->_URI + "\r\n");
	raw_response.append("\r\n");

	//Body
	raw_response.append(body);
    
    return ;

}

void	Response::construct200Deleted(void)
{
    if (body.empty())
	{
		body.append("<html>\r\n");
		body.append("<head><title>" + numberToString(client->status_code) + getErrorMessage(client->status_code) + "</title></head>\r\n");
		body.append("<body>\r\n");
		body.append("<center><h1> The file "
			   + this->request->_URI
			   + "was succesfully deleted</h1></center>\r\n");
        body.append("</body>\r\n");
		body.append("</html>\r\n");
	}

    //Status line
	raw_response.append("HTTP/1.1 ");
	raw_response.append(numberToString(client->status_code));
	raw_response.append(getErrorMessage(client->status_code));
	//Additional Info ??
	raw_response.append("\r\n");

	//Headers
	raw_response.append("Content-Length: " + numberToString(body.size()) + "\r\n");
	raw_response.append("Content-Type: text/html; charset=UTF-8\r\n");
	raw_response.append("\r\n");

	//Body
	raw_response.append(body);
    
    return ;

}
