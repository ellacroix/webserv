#include "Response.hpp"

Location *		Response::findLocation(std::string & uri)
{
	size_t		longestPrefixLen;
	Location *	bestMatchLoc;
	t_locMapIt	it;
	t_locMapIt	ite;

	longestPrefixLen = 0;
	bestMatchLoc = NULL;
	it = this->virtual_server->getLocationMap().begin();
	ite = this->virtual_server->getLocationMap().end();
	while (it != ite)
	{
		if (uri.find(it->first) != std::string::npos)
		{
			if (it->first.length() > longestPrefixLen)
			{
				longestPrefixLen = it->first.length();
				bestMatchLoc = it->second;
			}
		}
		it++;
	}
	return (bestMatchLoc);
}

void			Response::constructSuccess(void)
{
	std::string	line;

	this->request_file.open(this->path.c_str());
	if (this->request_file.good() == false)
	{
		this->status_code = 403;
		this->constructError();
		return ;
	}

	this->request_file.seekg (0, this->request_file.end);
	this->file_len = this->request_file.tellg();
	this->request_file.seekg (0, this->request_file.beg);

	this->raw_response.append("HTTP/1.1 OK Success\r\n");
	this->chunked = false;
	this->raw_response.append("Content-Length: ");
	this->raw_response.append(std::to_string(this->file_len));
	this->raw_response.append("\r\n");
	this->raw_response.append("Content-Type: text/html; charset=utf-8\r\n");
	this->raw_response.append("\r\n");
	while (std::getline(this->request_file, line))
		this->raw_response.append(line + '\n');
	this->request_file.close();
}

bool	Response::findIndex(void)
{
	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite;

	it = this->location->getIndex().begin();
	ite = this->location->getIndex().end();
	while (it != ite)
	{
		std::cout << "=== CHECKING " << this->path + *it << std::endl;
		if (pathExists(this->path + *it) == 1
				&& isFile(this->path + *it) == 1)
		{
			this->path.append(*it);
			std::cout << "=== FOUND INDEX "
				<< (*it) << " NEW PATH = "
				<< this->path << std::endl;
			return (true);
		}
		it++;
	}
	return (false);
}
