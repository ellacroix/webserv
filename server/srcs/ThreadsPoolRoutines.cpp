#include "webserv.hpp"
#include "Client.hpp"

void	threadRecvRoutine(Client *client, t_thread_info *thread_info)
{	
	printf("RECV_ROUTINE() - BUFFER : \"%s\"\n", client->request_buffer.c_str());
	//logger("Client " + numberToString(client->stream_socket) + " Recv routine");

	//The request is ignored, we monitor the connection again to read a new request
	if (client->request_buffer.find("\r\n") == 0)
	{
		logger("Client " + numberToString(client->stream_socket) + " blank line");
		client->request_buffer.clear();
		monitorForReading(client, thread_info);
		return ;
	}

	if (client->request_buffer.size() > SERVER_MAX_BODY_SIZE + SERVER_MAX_HEADERS_SIZE)
	{
		logger("Client " + numberToString(client->stream_socket) + " request too big");
		client->status_code = 431;
		createAndConstructResponse(client);
		monitorForWriting(client, thread_info);
		return ;
	}
	
	//We received the headers, is the request complete ?
	size_t end = client->request_buffer.find("\r\n\r\n");
	if (end != std::string::npos)
	{
		if (end > SERVER_MAX_HEADERS_SIZE)
		{
			logger("Client " + numberToString(client->stream_socket) + " headers too big");
			client->status_code = 431;
			createAndConstructResponse(client);
			monitorForWriting(client, thread_info);
			return ;
		}
		
		logger("Client " + numberToString(client->stream_socket) + " received all headers");
		if (!client->request)
			client->createRequest();

		//We analyze the headers to see if we are expecting a body, and if it is complete
		isRequestComplete(client);

		//We check if the body size is not too big
		if (client->request->_body.size() > SERVER_MAX_BODY_SIZE)
		{
			logger("Client " + numberToString(client->stream_socket) + " body too big");
			client->status_code = 413;
			createAndConstructResponse(client);
			monitorForWriting(client, thread_info);
			return ;
		}

		//The request is complete, we can parse it
		if (client->read_more == false)
		{
			logger("Client " + numberToString(client->stream_socket) + " request is complete, ready to parse");
			client->status_code = client->request->parser();
			createAndConstructResponse(client);
			monitorForWriting(client, thread_info);
			return ;
		}
	}

	//We need to read more data for the request to be complete
	logger("Client " + numberToString(client->stream_socket) + " request not complete");
	monitorForReading(client, thread_info);
}

void	threadSendRoutine(Client *client, t_thread_info *thread_info)
{	
	//logger("Client " + numberToString(client->stream_socket) + " Send routine");

	//Sending the reponse to the client
	int ret = send(client->stream_socket,
			client->response->raw_response.c_str(),
			client->response->raw_response.size(),
			0);
	if (ret <= 0)
	{
		logger("Client " + numberToString(client->stream_socket) + " closed the connection when writing to him\n");
		monitorForReading(client, thread_info);
		return ;
	}
	else
	{
		logger("Client " + numberToString(client->stream_socket) + " Send routine sent " + numberToString(ret) + " bytes");
		client->response->raw_response.erase(0, ret);
	}

	if (client->response->raw_response.size() == 0)
	{
		logger("Client " + numberToString(client->stream_socket) + " Send routine sent all the response " + numberToString(client->status_code));
		if (client->request)
			delete client->request;
		client->request = NULL;
		delete client->response;
		client->response = NULL;
		client->response_ready = false;
		client->request_buffer.clear();

		//Signal for main to disconnect the client and not monitor it again
		if (client->status_code == 408)
			client->connected = false;
		else
			monitorForReading(client, thread_info);
	}
	else
		monitorForWriting(client, thread_info);
}

void	*threadLoop(void* arg)
{
	t_thread_info *thread_info = (t_thread_info*)arg;
	Client *current_client;

	logger("Thread launched");

	while(true)
	{
		//Check if there is work to do in the queue or wait on cond_wait for work to be added
		pthread_mutex_lock(&thread_info->queue_mutex);
		if (thread_info->queue->empty() == true)
		{
			pthread_cond_wait(&thread_info->condition_var, &thread_info->queue_mutex);
			if (thread_info->queue->empty() == true)
			{
				pthread_mutex_unlock(&thread_info->queue_mutex);
				continue ;
			}
			current_client = thread_info->queue->front();
		}
		else
		{
			current_client = thread_info->queue->front();
		}
		thread_info->queue->pop_front();
		pthread_mutex_unlock(&thread_info->queue_mutex);

		//Determine which routine to do on the client
		pthread_mutex_lock(&current_client->client_mutex);
		if (current_client->suicide == true)
		{
			pthread_mutex_unlock(&current_client->client_mutex);
			return (0);
		}
		else if (current_client->response_ready == true)
			threadSendRoutine(current_client, thread_info);
		else
			threadRecvRoutine(current_client, thread_info);
		pthread_mutex_unlock(&current_client->client_mutex);
	}
}

void	toLowerHeaders(Client *client)
{
	size_t	start = 0;
	size_t	end = 0;
	unsigned int 	i = 0;

	while ((start = client->request->_headers.find("\r\n", start)) != std::string::npos)
	{
		start += 2;
		end = client->request->_headers.find(':', start);
		i = 0;

		while (end != std::string::npos && start + i < end)
		{
			if (std::isupper(client->request->_headers[start + i]))
				client->request->_headers[start + i] = std::tolower(client->request->_headers[start + i]);
			i++;
		}
	}
}

void	isRequestComplete(Client *client)
{	
	toLowerHeaders(client);
	
	size_t start = client->request->_headers.rfind("transfer-encoding: ");
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
	else if ((start = client->request->_headers.find("content-length: ")) != std::string::npos)
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
	size_t			body_start = client->request_buffer.find("\r\n\r\n") + 4;
	size_t			body_end = client->request_buffer.find("\r\n0\r\n", body_start);

	if (body_end != std::string::npos)
	{
		std::string body = client->request_buffer.substr(body_start, body_end + 5 - body_start);

		client->request->_body = body;
		
		// No trailer case
		if (client->request_buffer.find("\r\n0\r\n\r\n", body_end) != std::string::npos)
			return true;
		//Trailer case
		else if (client->request_buffer.find("\r\n\r\n", body_end + 5) != std::string::npos)
		{
			size_t	trailer_start = body_end + 5;
			size_t	trailer_end = client->request_buffer.find("\r\n\r\n", trailer_start);
			std::string headers = client->request_buffer.substr(trailer_start, trailer_end - trailer_start + 2);
			client->request->_headers.append(headers);
			return true;
		}
		return false;
	}
	else
		return false;
}

bool	isBodyComplete(Client *client, size_t length)
{
	size_t		body_start = client->request_buffer.find("\r\n\r\n") + 4;

	std::string	body = client->request_buffer.substr(body_start);

	if (body.size() < length)
		return false;
	else
		client->request->_body = body.substr(0, length);
	return true;
}

void	createAndConstructResponse(Client *client)
{
	client->createResponse();
	client->response->processRequest();
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
