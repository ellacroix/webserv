#include "Response.hpp"

bool	Response::isCgi(std::string path)
{
	std::string cgiExtension = client->request->_virtual_server->getCgiExtension();
	std::string fileExtension = findUriExtension(path);

	cgiExtension.erase(0,1);
	if (cgiExtension == fileExtension)
		return (true);
	return (false);
}

int	 Response::executeCgi()
{
	pid_t		pid;
	char  		**arg;
	char  		**env;
	int			fds[2];
	char		buffer[1024];
	int			count;
	int			code = 0;

	printf("=== CGI EXECUTION\n");
	printf("=== CGI - PATH\t= \"%s\"\n", this->path.c_str());
	printf("=== CGI - Q_S\t\t= \"%s\"\n", this->request->_query_string.c_str());
	printf("=== CGI - MET\t\t= \"%s\"\n", this->request->_method.c_str());
	if (pipe(fds) == -1)
		return (-1);

	arg = new char*[3];
	arg[0] = new char[client->request->_virtual_server->getCgiPath().length() + 1];
	arg[0] = strcpy(arg[0], client->request->_virtual_server->getCgiPath().c_str());
	arg[1] = new char[this->path.length() + 1];
	arg[1] = strcpy(arg[1], this->path.c_str());
	arg[2] = NULL;

	std::string	env_query_string;
	std::string env_method;
	std::string env_script_filename;
	std::string env_redirect_status;

	std::string env_server_protocol;
	std::string env_request_scheme;
	std::string env_request_uri;
	std::string env_document_uri;
	std::string env_document_root;

	env_query_string = "QUERY_STRING=" + this->request->_query_string;
	env_method = "REQUEST_METHOD=" + this->request->_method;
	env_script_filename = "SCRIPT_FILENAME=" + this->path;
	env_redirect_status = "REDIRECT_STATUS=200";

	env_server_protocol = "SERVER_PROTOCOL=HTTP/1.1";
	env_request_scheme = "REQUEST_SCHEME=http";
	env_request_uri = "REQUEST_URI=" + this->request->_URI + '?' + this->request->_query_string;
	env_document_uri = "DOCUMENT_URI=" + this->request->_URI;
	env_document_root = "DOCUMENT_ROOT=" + this->location->getRoot();
	env = new char*[10];
	env[0] = new char[env_query_string.length() + 1];
	env[0] = strcpy(env[0], env_query_string.c_str());
	env[1] = new char[env_method.length() + 1];
	env[1] = strcpy(env[1], env_method.c_str());
	env[2] = new char[env_script_filename.length() + 1];
	env[2] = strcpy(env[2], env_script_filename.c_str());
	env[3] = new char[env_redirect_status.length() + 1];
	env[3] = strcpy(env[3], env_redirect_status.c_str());
	/*
	env[4] = new char[env_server_protocol.length() + 1];
	env[4] = strcpy(env[4], env_server_protocol.c_str());
	env[5] = new char[env_request_scheme.length() + 1];
	env[5] = strcpy(env[5], env_request_scheme.c_str());
	env[6] = new char[env_request_uri.length() + 1];
	env[6] = strcpy(env[6], env_request_uri.c_str());
	env[7] = new char[env_document_uri.length() + 1];
	env[7] = strcpy(env[7], env_document_uri.c_str());
	env[8] = new char[env_document_root.length() + 1];
	env[8] = strcpy(env[8], env_document_root.c_str());
	env[9] = NULL;
	*/
	env[4] = NULL;

	pid = fork();
	if (pid == -1)
	{
		printf("=== CGI EXEC - FORK FAILED\n");
		delete [] arg[0];
		delete [] arg[1];
		delete [] arg;
		return (-1);
	}
	if (!pid)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);

		execve(arg[0], arg, env);
		printf("=== CGI EXEC - EXECVE FAILED\n");
		delete [] arg[0];
		delete [] arg[1];
		delete [] arg;
		exit(50); // LEAKS
	}
	else
	{
		close(fds[1]);
//		wait(&code);
		wait(&code);
		printf("=== CGI - PARENT - CHILD HAS RETURNED\n");
		if (WIFEXITED(code))
			printf("=== CGI - CHILD EXITED WITH %d STATUS\n", WEXITSTATUS(code));
		if (code/256 == 50)
			client->status_code = 500;
//		while ((count = read(fds[0], buffer, sizeof(buffer))) != 0)
		bzero(buffer, 1024);
		while ((count = read(fds[0], buffer, 1024)) != 0)
		{
			printf("=== CGI - PARENT - READING\n");
			buffer[count] = '\0';
			printf("=== CGI - READING - BUFFER :\n\"%s\"\n", buffer);
			printf("=== CGI - READING - COUNT = %d\n", count);
			body.append(buffer);
		}
		printf("=== CGI - PARENT - BODY WAS READ\n");
		close(fds[0]);
	}
	delete [] arg[0];
	delete [] arg[1];
	delete [] arg;
	return (0);
}
