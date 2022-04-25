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
	// std::FILE* bodyFile;
	std::fstream bodyFile;
	int fd;
	std::string responseBuffer;
	pid_t		pid;
	char  		**argv;
	char  		**env;
	int			fds[2];
	char		buffer[1024];
	int			count;
	int			code = 0;
	std::ofstream	tmp_body;

	printf("=== CGI EXECUTION\n");
	if (pipe(fds) == -1)
		return (-1);

	// ARG FOR EXECVE
	argv = new char*[3];
	argv[0] = ft_strdup(client->request->_virtual_server->getCgiPath());
	argv[1] = ft_strdup(this->path);
	argv[2] = NULL;

	// ENV FOR EXECVE
	env = new char*[11];
	env[0] = ft_strdup("QUERY_STRING=" + this->request->_query_string);
	env[1] = ft_strdup("REQUEST_METHOD=" + this->request->_method);
	env[2] = ft_strdup("SCRIPT_FILENAME=" + this->path);
	env[3] = ft_strdup("REDIRECT_STATUS=200");
	env[4] = ft_strdup("GATEWAY_INTERFACE=CGI/1.1");
	env[5] = ft_strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[6] = ft_strdup("CONTENT_LENGTH=" + numberToString(this->request->_body.length()));
	env[7] = ft_strdup("HTTP_ACCEPT=text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	if (this->request->_header_alrdy_set[CONTENT_TYPE] == true)
		env[8] = ft_strdup("CONTENT_TYPE=" + this->request->_content_type);
	//	env[8] = ft_strdup("CONTENT_TYPE=application/x-www-form-urlencoded");
	//	env[8] = ft_strdup("CONTENT_TYPE=multipart/form-data");
	env[9] = ft_strdup("BODY=" + this->request->_body);
	env[10] = NULL;
	printf("=== CGI - BODY :\n\"%s\"\n", this->request->_body.c_str());

	if (this->request->_method == "POST")
	{
		/* GOAL */
		// unsigned char* imgdata = ...;
		// size_t         imgdata_size = ...;

		// std::fstream imgout("MyImage.bin", std::out | std::binary);

		// imgout.write(reinterpret_cast<char*>(imgdata), imgdata_size);
		// imgout.close();
		/* ---------*/ 
	

		// size_t bodyImgSize;
		bodyFile.open("body_delete", std::ios::out | std::ios::binary);
		//
		// image.jpg expected
		// HTTP/1.1 200 OK
		// Content-Length: 109122
		// Content-Type: image/jpeg
		printf("LEN body_____ %lu\n", this->request->_body.length());
		// bodyImg.write()
		std::vector<char> bodyChr(this->request->_body.begin(), this->request->_body.end());
		
		// bodyFile = std::tmpfile(); // ERROR ?
		// printf("FILE________ [%s]\n", this->request->body)
		// bodyFile.write("salut");
		// LEN body_____ 109472
		// LEN body.c_str()_____ 167
		// LEN vector_____ 109472
		printf("LEN body.c_str()_____ %lu\n", strlen(this->request->_body.c_str()));
		printf("LEN vector_____ %lu\n", bodyChr.size());
		size_t len = bodyChr.size();
		size_t i;
		for (i = 0; i < len; i++)
		{
			bodyFile << bodyChr[i];
		}

			// write(bodyFile->_fileno, buf, 1);
		printf("LEN int %lu\n", len);
		printf("LEN WRITE I %lu\n", i);
			
		// std::fputs(bodyChr, bodyFile);
		//std::fputs(this->request->_body.c_str(), sizeof(char), body.length(), bodyFile);
		bodyFile.close();
	}

	fd = open("body_delete", O_RDONLY);
	pid = fork();
	if (pid == -1)
	{
		deleteArray(argv);
		deleteArray(env);
		return (-1);
	}
	if (!pid)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		if (this->request->_method == "POST")
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		close(fds[1]);

		execve(argv[0], argv, env);
		deleteArray(argv);
		deleteArray(env);
		exit(50); // LEAKS
	}
	else
	{
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
			printf("=== CGI - READING - BUFFER :\n\"%s\"\n", buffer);
			printf("=== CGI - READING - COUNT = %d\n", count);
			responseBuffer.append(buffer);
		}
		printf("=== CGI - PARENT - BODY WAS READ\n");
		close(fds[0]);
	}
	header = responseBuffer.substr(0, responseBuffer.find("\r\n\r\n") + 4);
	body = responseBuffer.substr(responseBuffer.find("\r\n\r\n") + 4, responseBuffer.length());

	deleteArray(argv);
	deleteArray(env);
	return (0);
}
