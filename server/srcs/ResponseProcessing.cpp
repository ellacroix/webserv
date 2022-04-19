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

bool	Response::findIndex(void)
{
	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite;

	it = this->location->getIndex().begin();
	ite = this->location->getIndex().end();
	while (it != ite)
	{
		//std::cout << "=== CHECKING " << this->path + *it << std::endl;
		if (pathExists(this->path + *it) == 1
				&& isFile(this->path + *it) == 1)
		{
			this->path.append(*it);
			//std::cout << "=== FOUND INDEX "
			//	<< (*it) << " NEW PATH = "
			//	<< this->path << std::endl;
			return (true);
		}
		it++;
	}
	return (false);
}

std::string	Response::findContentType(void)
{
	std::string	extension;
	const std::pair<std::string, std::string>	*pair_arr;

	//	extension = findUriExtension(this->request->_URI);
	extension = findUriExtension(this->path);

	if (extension.empty() == true)
		return ("application/octet-stream");
	pair_arr = &Response::document_type[0];
	while (pair_arr->first != "END")
	{
		if (pair_arr->first == extension)
		{
			//std::cout << "=== FOUND CONTENT-TYPE = " << pair_arr->second << std::endl;
			return (pair_arr->second);
		}
		pair_arr++;
	}
	return ("application/octet-stream");

}

int		Response::methodGET(void)
{
	//std::cout << "=== TESTING FILE/DIR = " << this->path << std::endl;
	if (pathExists(this->path) == false)
	{
		//std::cout << std::boolalpha;
		//std::cout << "=== PATH DOESN'T EXIST" << std::endl;
		//std::cout << "=== pathExists(this->path) == "
		//	<< pathExists(this->path) << std::endl;
		this->client->status_code = 404;
		this->constructError();
		return (SUCCESS);
	}
	if (canRead(this->path) == false)
	{
		this->client->status_code = 403;
		this->constructError();
		return (SUCCESS);
	}
	if (this->path[this->path.length() - 1] != '/')	//REQUEST A FILE
	{
		//std::cout << "=== REQUESTING A FILE" << std::endl;
		if (isDirectory(this->path) == false)
		{
			this->is_file = true;
			//std::cout << "=== FILE IS FOUND" << std::endl;
			this->client->status_code = 200;
			this->construct200Ok();
			return (SUCCESS);
		}
		else 
		{
			//std::cout << "=== FILE NOT FOUND" << std::endl;
			this->client->status_code = 404;
			this->constructError();
			return (SUCCESS);
		}
	}
	else											//REQUEST A DIR
	{
		//std::cout << "=== REQUESTING A DIRECTORY" << std::endl;
		//	CHECK index
		if (this->location->_indexIsSet == true)
		{
			//std::cout << "=== CHECKING INDEXES" << std::endl;
			// findIndex() WILL REPLACE path BY A NEW PATH AND RETURN TRUE
			// IF FOUND
			if (this->findIndex() == true)
			{
				//std::cout << "=== FOUND INDEX" << std::endl;
				if (canRead(this->path) == true)
				{
					this->client->status_code = 200;
					this->construct200Ok();
					return (SUCCESS);
				}
				else
				{
					this->client->status_code = 403;
					this->constructError();
					return (SUCCESS);
				}
			}
			else
				std::cout << "=== NO INDEX FOUND" << std::endl;
		}
		//	CHECK autoindex
		if (this->location->_autoIndexIsSet == true
				&& this->location->getAutoIndex() == true)
		{
			//std::cout << "=== CONSTRUCTING AUTOINDEX" << std::endl;
			this->client->status_code = 200;
			this->constructAutoIndex();
			return (SUCCESS);
		}
		else
		{
			this->client->status_code = 404;
			this->constructError();
			return (SUCCESS);
		}
	}
}

int		Response::methodPOST(void)
{
	std::string	path_to_folder;
	size_t		last_slash_pos;
//	std::string	def_upload_name;
//	size_t		n;

	if (this->path[this->path.length() - 1] != '/')		//REQUEST A FILE
	{
		std::cout << "=== POST - REQUEST TO FILE" << std::endl;
		//	FILE ALREADY EXISTS
		if (pathExists(this->path) == true)
		{
			std::cout << "=== POST - PATH EXISTS" << std::endl;
			if (isFile(this->path) == false)
			{
				std::cout << "=== POST - PATH IS NOT A FILE\t=> 403" << std::endl;
				this->client->status_code = 403;
				this->constructError();
				return (SUCCESS);
			}
			if (canWrite(this->path) == false)
			{
				std::cout << "=== POST - NO WRITE PERMISSION FOR FILE\t=> 403"
					<< std::endl;
				this->client->status_code = 403;
				this->constructError();
				return (SUCCESS);
			}
		}
		//	FILE DOESN'T EXIST YET
		else	
		{
			std::cout << "=== POST - PATH DOESN'T EXIST" << std::endl;
			last_slash_pos = this->path.find_last_of("/");
			path_to_folder = this->path.substr(0, last_slash_pos);
			std::cout << "=== POST - CONTAINING FOLDER : \""
				<< path_to_folder << "\"" << std::endl;
			if (pathExists(path_to_folder) == false)
			{
				std::cout << "=== POST - CONTAINING FOLDER DOESN'T EXIST\t=> 404"
					<< std::endl;
				this->client->status_code = 404;
				this->constructError();
				return (SUCCESS);
			}
			if (isDirectory(path_to_folder) == false)
			{
				std::cout << "=== POST - CONTAINING FOLDER IS A FILE\t=> 404"
					<< std::endl;
				this->client->status_code = 404;
				this->constructError();
				return (SUCCESS);
			}
			if (canWrite(path_to_folder) == false)
			{
				std::cout << "=== POST - CANNOT WRITE TO CONTAINING FOLDER\t=> 403"
					<< std::endl;
				this->client->status_code = 403;
				this->constructError();
				return (SUCCESS);
			}
		}
		std::cout << "=== POST - CREATING FILE" << std::endl;
		if (this->createFile() != SUCCESS)
			return (this->client->status_code);
		std::cout << "=== POST - CREATED FILE\t=> 201" << std::endl;
		this->client->status_code = 201;
		this->construct201Created();
		return (SUCCESS);
	}
	else											//REQUEST A DIR
	{
		std::cout << "=== POST - REQUEST TO FOLDER\t=> 403" << std::endl;
		this->client->status_code = 403;
		this->constructError();
		return (SUCCESS);
		/*
		// NEED FOLDER WRITE PERM CHECK
		// OR => 403
		def_upload_name = "upload_";

		n = 1;
		while (pathExists(this->path + def_upload_name
					+ numberToString(n)
					+ findUriExtension(this->path)))
			n++;
		this->path += def_upload_name
			+ numberToString(n) + findUriExtension(this->path);
		std::cout << "=== POST - CREATING FILE" << std::endl;
		if (this->createFile() != SUCCESS)
			return (this->client->status_code);
		std::cout << "=== POST - CREATED FILE\t=> 201" << std::endl;
		this->client->status_code = 201;
		this->construct201Created();
		return (SUCCESS);
		*/
	}
}
