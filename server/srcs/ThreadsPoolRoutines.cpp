#include "webserv.hpp"
#include "Client.hpp"

void	thread_recv_routine(Client *client, t_thread_info *thread_info)
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
		client->statusCode = 431;
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
			client->CreateRequest();

		//We analyze the headers to see if we are expecting a body, and if it is complete
		isRequestComplete(client);

		//We check if the body size is not too big
		if (client->request->_body.size() > SERVER_MAX_BODY_SIZE)
		{
			client->statusCode = 413;
			createAndConstructResponse(client);
			monitorForWriting(client, thread_info);
			return ;
		}

		//The request is complete, we can parse it
		if (client->read_more == false)
		{
			client->statusCode = client->request->parser();
			createAndConstructResponse(client);
			monitorForWriting(client, thread_info);
			return ;
		}
	}

	//We need to read more data for the request to be complete
	monitorForReading(client, thread_info);
}

void	thread_send_routine(Client *client, t_thread_info *thread_info)
{	
	printf("ThreadsPool: send routine\n");

	int ret = 0;
	int size = client->response->raw_response.size();
	int	iter;

	//Sending the reponse to the client
	iter = 0;
	while (size > 0)
	{
		ret = send(client->stream_socket,
				client->response->raw_response.c_str() + ret,
				size,
				0);
		if (ret < 1)
			printf("Strange error\n");
		size -= ret;
		iter++;
	}
	std::cout << "=== SENT IN " << iter << " ITERATIONS" << std::endl;

	if (client->request)
		delete client->request;
	client->request = NULL;
	delete client->response;
	client->response = NULL;
	client->response_ready = false;
	client->request_buffer.clear();

	//Signal for main to disconnect the client and not monitor it again
	if (client->statusCode == 408)
		client->connected = false;
	else
		monitorForReading(client, thread_info);

	printf("ThreadsPool: send routine DONE\n");
}

void	*thread_loop(void* arg)
{
	t_thread_info *thread_info = (t_thread_info*)arg;
	Client *currentClient;

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
		if (currentClient->response_ready == true)
			thread_send_routine(currentClient, thread_info);
		else
			thread_recv_routine(currentClient, thread_info);
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
			if (IsChunkComplete(client) == false)
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
		if (IsBodyComplete(client, length) == false)
			client->read_more = true;
		else
			client->read_more = false;
		}
}

bool	IsChunkComplete(Client *client)
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

bool	IsBodyComplete(Client *client, size_t length)
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
	client->CreateResponse();
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
