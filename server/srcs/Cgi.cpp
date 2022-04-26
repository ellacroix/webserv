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

char	**setCgiEnvironnement(Request *request, std::string path)
{
	char **env;
	env = new char*[9];
	env[0] = ft_strdup("QUERY_STRING=" + request->_query_string);
	env[1] = ft_strdup("REQUEST_METHOD=" + request->_method);
	env[2] = ft_strdup("SCRIPT_FILENAME=" + path);
	env[3] = ft_strdup("REDIRECT_STATUS=200");
	env[4] = ft_strdup("CONTENT_LENGTH=" + numberToString(request->_body.length()));
	env[5] = ft_strdup("CONTENT_TYPE=" + request->_content_type);
	env[6] = ft_strdup("BODY=" + request->_body);
	env[7] = NULL;

	return (env);
}

int getBodyFileFd(std::string filename, std::string body)
{
	std::ofstream	bodyFile;
	int	fd;

	bodyFile.open(filename.c_str(), std::ios::out | std::ios::binary);
	if (!bodyFile.good())
		return (-1);
	bodyFile.write(body.c_str(), body.length());	
	bodyFile.close();

	if ((fd = open(filename.c_str(), O_RDONLY)) == -1)
		return (-1);
	return (fd);
}

void	readResponseBuffer(std::string & header, std::string & body, int fd)
{
	int count;
	char buffer[1024];
	std::string responseBuffer;

	(void)header;
	(void)body;
	while ((count = read(fd, buffer, sizeof(buffer))) != 0)
	{
		buffer[count] = '\0';
		responseBuffer.append(buffer);
	}

	header = responseBuffer.substr(0, responseBuffer.find("\r\n\r\n") + 4);
	body = responseBuffer.substr(responseBuffer.find("\r\n\r\n") + 4, responseBuffer.length());
}

int		executeBinaryCgi(char **argv, char **env, int *fds, int bodyFd, std::string method)
{
	int pid;
	int code = 0;

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

		if (method == "POST")
		{
			dup2(bodyFd, STDIN_FILENO);
			close(bodyFd);
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
		if (method == "POST")
			close(bodyFd);
	}
	return (code);
}


int	 Response::executeCgi()
{
	std::string		filename;
	// std::string		respoeBuffer;
	char  			**argv;
	char  			**env;
	int				fds[2];
	int				code;
	int				fd = -1;

	printf("=== CGI EXECUTION\n");

	if (pipe(fds) == -1)
		return (-1);

	// WRITE REQUEST BODY IN FILE
	if (this->request->_method == "POST")
	{
		filename = "/tmp/webserv_body_" + numberToString(syscall(__NR_gettid));
		if ((fd = getBodyFileFd(filename, this->request->_body)) == -1)
			return (-1);
	}

	// SET CGI PARAMS
	argv = new char*[2];
	argv[0] = ft_strdup(client->request->_virtual_server->getCgiPath());
	argv[1] = NULL;

	env = setCgiEnvironnement(this->request, this->path);

	code = executeBinaryCgi(argv, env, fds, fd, this->request->_method);
	if (code/256 == 50)
		client->status_code = 500;

	readResponseBuffer(this->header, this->body, fds[0]);

	deleteArray(argv);
	deleteArray(env);
	return (0);
}

