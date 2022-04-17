#include "Response.hpp"

void            Response::constructSuccess(void)
{
    std::string line;
	std::string	extension;
	char		*buffer;

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

	//	STATUS LINE
    this->raw_response.append("HTTP/1.1 ");
	this->raw_response.append(numberToString(this->client->status_code));
	this->raw_response.append(getErrorMessage(this->client->status_code) + "\r\n");
	//	HEADERS
    this->raw_response.append("Content-Length: ");
	if (this->isCgi(this->path))
	{
		printf("ABOUT SIZE ______________ %d\n", sizeof(body));
		printf("ABOUT SIZE ______________ %d\n", body.size());
    	this->raw_response.append(numberToString(sizeof(body)));
		this->raw_response.append("\r\n");
	}
	else
		this->raw_response.append(numberToString(this->file_len) + "\r\n");
    this->raw_response.append("Content-Type: ");
	this->raw_response.append(this->findContentType() + "\r\n");
    this->raw_response.append("\r\n");

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
