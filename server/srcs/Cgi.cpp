#include "Response.hpp"

bool	Response::isCgi(std::string path)
{
	std::string cgiExtension = client->request->_virtual_server->getCgiExtension();
	std::string fileExtension = findUriExtension(path);

	cgiExtension.erase(0,1);
	if (cgiExtension == fileExtension && isFile(this->path))
		return (true);
	return (false);
}

int	 Response::executeCgi()
{
	pid_t		pid;
	char  		**arg;
	int			fds[2];
	char		buffer[1024];
	int			count;

	printf("=== CGI EXECUTION\n");
	if (pipe(fds) == -1)
	{
		std::cerr << "error: execve failed" << std::endl; // need to change err msg FREE
		return (-1);
	}

	arg = new char*[3];
	arg[0] = new char[client->request->_virtual_server->getCgiPath().length() + 1];
	arg[0] = strcpy(arg[0], client->request->_virtual_server->getCgiPath().c_str());
	arg[1] = new char[this->path.length() + 1];
	arg[1] = strcpy(arg[1], this->path.c_str());
	arg[2] = NULL;

	pid = fork();
	if (pid == -1)
	{
		delete [] arg[0];
		delete [] arg[1];
		std::cerr << "error: fork failed" << std::endl; // need to change err msg
		return (-1);
	}
	if (!pid)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);

		execve(arg[0], arg, NULL); //need to exit and free if fail

		std::cerr << "error: execve failed" << std::endl;
		exit(1);
	}
	else
	{
		close(fds[1]);
		wait(NULL);
		while ((count = read(fds[0], buffer, sizeof(buffer))) != 0)
		{
			buffer[count] = '\0';
			body.append(buffer);
		}
		std::size_t find = body.find("\r\n\r\n");
		body.erase(body.begin(), body.begin() + find + 4);

		close(fds[0]);
	}
	delete [] arg[0];
	delete [] arg[1];
	return (0);
}
