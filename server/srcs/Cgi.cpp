#include "Response.hpp"

bool   Response::isCgi(std::string path)
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
	std::ofstream	bodyFile;
	std::string		responseBuffer;
	pid_t			pid;
	char  			**argv;
	char  			**env;
	int				fds[2];
	char			buffer[1024];
	int				count;
	int				code = 0;
	int				fd;

	printf("=== CGI EXECUTION\n");

	if (pipe(fds) == -1)
		return (-1);

	// WRITE REQUEST BODY IN FILE
	if (this->request->_method == "POST")
	{
		bodyFile.open("/tmp/webserv-body", std::ios::out | std::ios::binary);
		if (!bodyFile.good())
			return (-1);
		bodyFile.write(this->request->_body.c_str(), this->request->_body.length());	
		bodyFile.close();
		if ((fd = open("/tmp/webserv-body", O_RDONLY)) == -1)
			return (-1);	
	}

	// SET CGI PARAMS
	argv = new char*[2];
	argv[0] = ft_strdup(client->request->_virtual_server->getCgiPath());
	argv[1] = NULL;

	env = new char*[9];
	env[0] = ft_strdup("QUERY_STRING=" + this->request->_query_string);
	env[1] = ft_strdup("REQUEST_METHOD=" + this->request->_method);
	env[2] = ft_strdup("SCRIPT_FILENAME=" + this->path);
	env[3] = ft_strdup("REDIRECT_STATUS=200");
	env[4] = ft_strdup("CONTENT_LENGTH=" + numberToString(this->request->_body.length()));
	env[5] = ft_strdup("CONTENT_TYPE=" + this->request->_content_type);
	env[6] = ft_strdup("BODY=" + this->request->_body);
	env[7] = NULL;

	if ((pid = fork()) == -1)
	{
		deleteArray(argv);
		deleteArray(env);
		return (-1);
	}
	else if (!pid)
	{
		// IN CHILD: EXEC CGI BINARY AND OPTIONALLY CONNECT FILE WITH BODY REQUEST CONTENT TO STDIN
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);

		if (this->request->_method == "POST")
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}

		execve(argv[0], argv, env);
		deleteArray(argv);
		deleteArray(env);
		exit(50); // LEAKS
	}
	else
	{
		// IN PARENT: READ THE OUTPUT OF THE CGI BINARY EXECUTION
		close(fds[1]);
		wait(&code);
		if (this->request->_method == "POST")
			close(fd);
		if (WIFEXITED(code))
			printf("=== CGI - CHILD EXITED WITH %d STATUS\n", WEXITSTATUS(code));
		if (code/256 == 50)
			client->status_code = 500;
		bzero(buffer, 1024);
		while ((count = read(fds[0], buffer, sizeof(buffer))) != 0)
		{
			buffer[count] = '\0';
			responseBuffer.append(buffer);
		}
		close(fds[0]);
	}

	// PARSE OUTPUT AND WRITE RESPONSE HEADER+BODY
	header = responseBuffer.substr(0, responseBuffer.find("\r\n\r\n") + 4);
	body = responseBuffer.substr(responseBuffer.find("\r\n\r\n") + 4, responseBuffer.length());

	deleteArray(argv);
	deleteArray(env);
	return (0);
}
