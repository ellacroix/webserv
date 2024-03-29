#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "webserv.hpp"

#include "Client.hpp"
#include "Request.hpp"
#include "VirtualServer.hpp"

class Client ;
class Request ;
class VirtualServer ;

class Response {

private:
	static const std::pair<std::string, std::string>document_type[105];

protected:

public:
	//	DATA RESPONSE DEPENDS ON
	Client			*client;
	Request			*request;
	VirtualServer	*virtual_server;
	Location		*location;

	std::string		path;
	//	RESPONSE DATA
	std::string		raw_response;
	std::string		body;
	std::string		header;
	int				status_code;
	std::ifstream	request_file;
	std::ofstream	post_file;
	size_t			file_len;
	bool			chunked;
	bool			is_file;
	bool			error_directive;

	//	HEADERS
	std::string		location_header;
	

	//	CONSTRUCTOR / DESTRUCTOR
	Response() {};
	~Response() {};
	Response(Client *parent_client);

	//	RESPONSE PROCESSING
	int			processRequest();
	void		constructError();
	std::string	getErrorMessage(int code);

	//	FILE - ResponseProcessing.cpp
	bool			findIndex(void);
	std::string		findContentType(void);
	void			constructAutoIndex();
	int				methodGET(void);
	int				methodPOST(void);
	int				methodDELETE(void);
	Location *		findLocation(std::string & URI);

	//	FILE - ResponseConstruction.cpp
	void		construct200Ok(void);
	void		constructCgi(void);
	void		construct200Deleted(void);
	void		construct201Created(void);
	void		constructRedirection(void);
	int			createFile(void);


	//	FILE - Cgi.cpp
	int			executeCgi(void);
	bool		isCgi(std::string path);

};

#endif
