#include "Response.hpp"
#include <sys/types.h>
#include <dirent.h>

Response::Response(Client *parent_client)
{
	client = parent_client;
	request = parent_client->request;
	//status_code = parent_client->request->_status_code;
	status_code = parent_client->status_code;
	if (request != NULL)
	{
		virtual_server = parent_client->request->_virtual_server;
	}
}

int	Response::ConstructResponse()
{	
 	//	FIND VIRTUAL SERVER IF NO REQUEST WAS CREATED
	if (this->request == NULL)
		this->virtual_server = client->parent_port->_VS_list.front();

	if (client->status_code != 0)
	{
	//	IF ERROR DETECTED IN PARSING
		constructError();
		return (SUCCESS);
	}
	//	ELSE (PROCESSING CONTINUES)
	//	constructError();
	//	FIND location
	this->location = this->findLocation(this->request->_URI);
	if (this->location == NULL)
	{
		this->client->status_code = 404;
		this->constructError();
		return (SUCCESS);
	}
	std::cout << "=== FOUND LOCATION = "
		<< this->location->getPrefix() << std::endl;
	//	CHECK limit_except
	if (this->location->_limitExceptIsSet == true &&
			std::find(this->location->getLimitExcept().begin(),
				this->location->getLimitExcept().end(),
				this->request->_method)
			!= this->location->getLimitExcept().end())
	{
		this->client->status_code = 405;
		this->constructError();
		return (SUCCESS);
	}
	/*
	//	CHECK return
	if (this->location->_returnIsSet == true)
	{
	this->client->status_code = this->location->getReturnCode();
	this->location_header = this->location->getReturnUri();
	this->constructRedirection();
	return (SUCCESS);
	}
	*/
	//	MAKE DIR/FILE PATH	- ROOT VERSION
	//this->path = this->location.getRoot().append(this->request->_URI);
	//						- ALIAS VERSION
	this->path = this->request->_URI;
	this->path.replace(0, this->location->getPrefix().length(),
			this->location->getRoot());
	
	if (this->request->_method == "GET")
		this->methodGET();
	/*
	else if (this->request->method == "POST")

	else if (this->request->method == "DELETE")
	*/


	else
	{
		this->client->status_code = 404;
		this->constructError();
	}
	return (SUCCESS);
}

std::string fillTag(std::string balise, std::string str)
{
	std::string tag;
	std::string content;

	for (int i = 0; balise[i] && balise[i] != ' '; i++) // need to check all whitespace
		tag += balise[i];
	content += "<" + balise + ">" + str + "</" + tag + ">";
	return (content);
}

std::vector<std::string> getDirectoryContent( std::string pathDir, std::string path)
{
	std::vector<std::string> dirLst;
	struct dirent 	*entry;
	DIR *dir =		opendir(path.c_str());
	struct stat 	info;
    struct tm * 	timeinfo;
	std::string 	entryLink;
	std::string fullPath;
	std::string entryPath;
	char buffer[256]; // need to reset

	std::string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
							"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	while ((entry = readdir(dir)) != NULL)
	{
		fullPath = path + "/" + entry->d_name;
		stat(fullPath.c_str(), &info);

		if (!std::strcmp(entry->d_name,  "."))
			continue ;
		else if (!std::strcmp(entry->d_name,  ".."))
		{
			entryLink = fillTag("h1", "Index of " + pathDir);
			entryLink += "<hr><pre>";
			entryLink += fillTag("a href=\"../\"", "../");
			dirLst.push_back(entryLink);
		}
		else
		{
			entryPath = entry->d_name;
			if (entry->d_type == DT_DIR) { entryPath += "/"; }
			entryLink = fillTag("a href=\"" + entryPath + "\"", entryPath);

			// for (int i = entryLink.length(); entryLink.length() < 51; i++)
			//     entryLink += " ";
			for (int i = 0; entryLink.length() + i < 109; i++)
				entryLink += " ";

			timeinfo = gmtime(&info.st_ctime);
			strftime(buffer, 18, "%d-%b-%Y %H:%M", timeinfo);
			entryLink += buffer;

			for (int i = 0; i < 21; i++)
				entryLink += " ";

			if (entry->d_type == DT_DIR) { entryLink += "-"; }
			else
			{
				std::stringstream stream;  
				stream << info.st_size;
				entryLink += stream.str();
			}
			dirLst.push_back(entryLink);
		}
	}
	closedir(dir);
	return (dirLst);
}

void	Response::constructAutoIndex()
{
	std::vector<std::string> dirLst;

	if (body.empty())
	{
		body.append("<html>\r\n");
		body.append(fillTag("head", fillTag("title", "Index of " + client->request->_URI)));
		body.append("\r\n<body>\r\n");
		dirLst = getDirectoryContent(client->request->_URI, this->path.c_str());
		// need to replace with real path but segfault actually
		for (unsigned int i = 0; i < dirLst.size(); i++) {
			body.append(dirLst.at(i)); 
			body.append("\r\n");
		}
		body.append("</pre><hr></body>\r\n");
		body.append("</html>\r\n");
	}

	raw_response.append("HTTP/1.1 ");
	raw_response.append(numberToString(client->status_code));
	raw_response.append("\r\n");

	raw_response.append("Content-Length: " + numberToString(body.size()) + "\r\n");
	raw_response.append("Content-Type: text/html; charset=UTF-8\r\n");
	raw_response.append("\r\n");

	raw_response.append(body);

	std::cout << "-----------------------------------------------------------\n";
}


void	Response::constructError()
{
 	//if we don't find _status_code in a std::map<code, File>, we send the default error
	if (body.empty())
	{
		printf("Redacting default page\n");
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
