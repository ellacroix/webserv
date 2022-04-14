#include "webserv.hpp"
#include "Client.hpp"
#include "Logger.hpp"

void	threadRecvRoutine(Client *client, t_thread_info *thread_info)
{	
	printf("ThreadsPool: recv routine \n");

	//The request is ignored, we monitor the connection again to read a new request
	if (client->request_buffer.find("\r\n") == 0)
	{
		client->request_buffer.clear();
		monitorForReading(client, thread_info);
		return ;
	}

	if (client->request_buffer.size() > SERVER_MAX_HEADERS_SIZE)
	{
		client->status_code = 431;
		createAndConstructResponse(client);
		monitorForWriting(client, thread_info);
		return ;
	}
	
	//We received the headers, is the request complete ?
	size_t end = client->request_buffer.find("\r\n\r\n");
	if (end != std::string::npos)
	{
		printf("Received all headers\n");
		if (!client->request)
			client->createRequest();

		//We analyze the headers to see if we are expecting a body, and if it is complete
		isRequestComplete(client);

		//We check if the body size is not too big
		if (client->request->_body.size() > SERVER_MAX_BODY_SIZE)
		{
			client->status_code = 413;
			createAndConstructResponse(client);
			monitorForWriting(client, thread_info);
			return ;
		}

		//The request is complete, we can parse it
		if (client->read_more == false)
		{
			client->status_code = client->request->parser();
			createAndConstructResponse(client);
			monitorForWriting(client, thread_info);
			return ;
		}
	}

	//We need to read more data for the request to be complete
	monitorForReading(client, thread_info);
}

void	threadSendRoutine(Client *client, t_thread_info *thread_info)
{	
	printf("ThreadsPool: send routine\n");

	//Sending the reponse to the client
	int ret = send(client->stream_socket,
			client->response->raw_response.c_str(),
//			1,
			client->response->raw_response.size(),
			0);
	if (ret < 0)
		printf("ThreadsPool: Client closed the connection when writing to him\n");
	else
		client->response->raw_response.erase(0, ret);

	if (client->response->raw_response.size() == 0 || ret < 0)
	{
		if (client->request)
			delete client->request;
		client->request = NULL;
		delete client->response;
		client->response = NULL;
		client->response_ready = false;
		client->request_buffer.clear();
		printf("ThreadsPool: send routine sent all the response\n");

		//Signal for main to disconnect the client and not monitor it again
		if (client->status_code == 408)
			client->connected = false;
		else
			monitorForReading(client, thread_info);
	}
	else
	{
		printf("ThreadsPool: send routine sent partial response\n");
		monitorForWriting(client, thread_info);
	}
}

void	*threadLoop(void* arg)
{
	t_thread_info *thread_info = (t_thread_info*)arg;
	Client *currentClient;

	pid_t x = syscall(__NR_gettid);
	Logger	thread_log(numberToString(x));

	printf("ThreadsPool: Thread launched\n");

	while(true)
	{
		//Check if there is work to do in the queue or wait on cond_wait for work to be added
		pthread_mutex_lock(&thread_info->queue_mutex);
		if (thread_info->queue->empty() == true)
		{
			printf("ThreadsPool: No work in the queue, waiting...\n");
			pthread_cond_wait(&thread_info->condition_var, &thread_info->queue_mutex);
			currentClient = thread_info->queue->front();
		}
		else
			currentClient = thread_info->queue->front();
		//printf("ThreadsPool: Grabbed a task\n");
		thread_info->queue->pop_front();
		pthread_mutex_unlock(&thread_info->queue_mutex);

		//Determine which routine to do on the client
		pthread_mutex_lock(&currentClient->client_mutex);
		if (currentClient->suicide == true)
		{
			pthread_mutex_unlock(&currentClient->client_mutex);
			return (0);
		}
		else if (currentClient->response_ready == true)
			threadSendRoutine(currentClient, thread_info);
		else
			threadRecvRoutine(currentClient, thread_info);
		pthread_mutex_unlock(&currentClient->client_mutex);
	}
}

void	isRequestComplete(Client *client)
{
	size_t start = client->request->_headers.rfind("Transfer-Encoding: ");
	if (start != std::string::npos)
	{
		std::string TE_line = client->request->_headers.substr(start);
		TE_line = TE_line.substr(0, TE_line.find("\r\n"));
		if (TE_line.find("chunked") != std::string::npos)
		{
			if (isChunkComplete(client) == false)
				client->read_more = true;
			else
				client->read_more = false;
		}
	}
	else if ((start = client->request->_headers.find("Content-Length: ")) != std::string::npos)
	{
		std::string CL_line = client->request->_headers.substr(start);
		CL_line = CL_line.substr(CL_line.find(":") + 1, CL_line.find("\r\n") + 2);
		size_t length = strtol(CL_line.c_str(), NULL, 10);
		if (isBodyComplete(client, length) == false)
			client->read_more = true;
		else
			client->read_more = false;
		}
}

bool	isChunkComplete(Client *client)
{
	size_t			bodyStart = client->request_buffer.find("\r\n\r\n") + 4;
	size_t			bodyEnd = client->request_buffer.find("\r\n0\r\n", bodyStart);

	if (bodyEnd != std::string::npos)
	{
		std::string body = client->request_buffer.substr(bodyStart, bodyEnd + 5 - bodyStart);

		client->request->_body = body;
		
		// No trailer case
		if (body.find("\r\n0\r\n\r\n") != std::string::npos)
			return true;
		//Trailer case to do ?
		return true;
	}
	else
		return false;
}

bool	isBodyComplete(Client *client, size_t length)
{
	size_t		bodyStart = client->request_buffer.find("\r\n\r\n") + 4;

	std::string	body = client->request_buffer.substr(bodyStart);

	if (body.size() < length)
		return false;
	else
		client->request->_body = body.substr(0, length);
	return true;
}

void	createAndConstructResponse(Client *client)
{
	client->createResponse();
	client->response->ConstructResponse();
}

void	monitorForReading(Client *client, t_thread_info *thread_info)
{
	//The request is incomplete, we monitor the connection again to read the rest
	pthread_mutex_lock(&thread_info->epoll_fd_mutex);
	thread_info->event.data.fd = client->stream_socket;
	thread_info->event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	epoll_ctl(*thread_info->epoll_fd,
			EPOLL_CTL_MOD,
			client->stream_socket,
			&thread_info->event);
	pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
}

void	monitorForWriting(Client *client, t_thread_info *thread_info)
{
	//Response is ready to be sent, so we monitor client->stream_socket for writing only
	pthread_mutex_lock(&thread_info->epoll_fd_mutex);
	thread_info->event.data.fd = client->stream_socket;
	thread_info->event.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
	epoll_ctl(*thread_info->epoll_fd, EPOLL_CTL_MOD, client->stream_socket, &thread_info->event);
	client->response_ready = true;
	pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
}
