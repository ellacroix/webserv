#include "Response.hpp"

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
	error_directive = false;
}

int	Response::processRequest()
{	
 	//	FIND VIRTUAL SERVER IF NO REQUEST WAS CREATED
	if (this->request == NULL || this->virtual_server == NULL)
		this->virtual_server = client->parent_port->_VS_list.front();

	//	IF ERROR DETECTED IN PARSING
	if (client->status_code != 0)
	{
		constructError();
		return (SUCCESS);
	}
	
	//	ELSE (PROCESSING CONTINUES)
	//	FIND location
	this->location = this->findLocation(this->request->_URI);
	// if (this->location == NULL && !this->isCgi(request->_URI))
	if (this->location == NULL)
	{
		this->client->status_code = 404;
		this->constructError();
		return (SUCCESS);
	}
	//std::cout << "=== FOUND LOCATION = "
	//	<< this->location->getPrefix() << std::endl;

	//	CHECK limit_except
	if (this->location->_limitExceptIsSet == true &&
			std::find(this->location->getLimitExcept().begin(),
				this->location->getLimitExcept().end(),
				this->request->_method)
			== this->location->getLimitExcept().end())
	{
		this->client->status_code = 405;
		this->constructError();
		return (SUCCESS);
	}

	//	MAKE DIR/FILE PATH
	this->path = this->request->_URI;
	this->path.replace(0, this->location->getPrefix().length(),
			this->location->getRoot());

	// EXECUTION CGI
	if (this->isCgi(this->path))
	{
		if (executeCgi() == -1)
		{
			this->client->status_code = 505;
			this->constructError();
			return (SUCCESS);
		}
	}
	
	//	CHECK return
	if (this->location->_returnIsSet == true)
	{
		this->client->status_code = this->location->getReturnCode();
		this->location_header = this->location->getReturnUri();
		this->constructRedirection();
		return (SUCCESS);
	}
	
	if (this->request->_method == "GET")
		this->methodGET();
	else if (this->request->_method == "POST")
		this->methodPOST();
	else if (this->request->_method == "DELETE")
		this->methodDELETE();

	else
	{
		this->client->status_code = 505;
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
		printf("full path %s\n", fullPath.c_str());

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

	//std::cout << "-----------------------------------------------------------\n";
}
