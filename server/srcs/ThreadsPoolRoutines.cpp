#include "webserv.hpp"
#include "Client.hpp"

bool	IsChunkComplete(Client *client)
{
	size_t			bodyStart = client->request_buffer.find("\r\n\r\n") + 4;
	size_t			bodyEnd = client->request_buffer.find("\r\n0\r\n", bodyStart);

	if (bodyEnd != std::string::npos)
	{
		std::string body = client->request_buffer.substr(bodyStart, bodyEnd + 5 - bodyStart);

		client->request->body = body;
		
		// No trailer case
		if (body.find("\r\n0\r\n\r\n") != std::string::npos)
			return true;
		//Trailer case to do ?
		return true;
	}
	else
		return false;

/* 	std::string		nextLine = body.substr(0, 100);
	int				chunksize = strtol(nextLine.c_str(), NULL, 16);
	size_t			chunkstart = 0;
	//size_t			len = body.length();
	std::string		chunk;

	while (chunksize != 0)
	{
//		if (chunkstart + chunksize > len)
//			ON VA SORTIR DE LA body
		chunkstart = body.find("\r\n", chunkstart) + 2;
		chunk = body.substr(chunkstart, chunksize);
		printf("chunk: %s\n", chunk.c_str());
		nextLine = body.substr(chunkstart + chunksize + 2, 100);
		chunkstart += chunksize + 2;
		chunksize = strtol(nextLine.c_str(), NULL, 16);
	} */

}

bool	IsBodyComplete(Client *client, size_t length)
{
	size_t		bodyStart = client->request_buffer.find("\r\n\r\n") + 4;

	std::string	body = client->request_buffer.substr(bodyStart);

	if (body.size() < length)
		return false;
	else
		client->request->body = body.substr(0, length);
	return true;
}

void	thread_recv_routine(Client *client, t_thread_info *thread_info)
{	
	printf("ThreadsPool: recv routine: ");

	size_t end = client->request_buffer.find("\r\n\r\n");
	if (end != std::string::npos)
	{
		printf("Received all headers\n");
		if (!client->request)
			client->CreateRequest();

		size_t start = client->request->headers.rfind("Transfer-Encoding: ");
		if (start != std::string::npos)
		{
			std::string TE_line = client->request->headers.substr(start);
			TE_line = TE_line.substr(0, TE_line.find("\r\n"));
			if (TE_line.find("chunked") != std::string::npos)
			{
				if (IsChunkComplete(client) == false)
					client->read_more = true;
				else
					client->read_more = false;
			}
		}
		else if ((start = client->request->headers.find("Content-Length: ")) != std::string::npos)
		{
			std::string CL_line = client->request->headers.substr(start);
			CL_line = CL_line.substr(CL_line.find(":") + 1, CL_line.find("\r\n") + 2);
			size_t length = strtol(CL_line.c_str(), NULL, 10);
			if (IsBodyComplete(client, length) == false)
				client->read_more = true;
			else
				client->read_more = false;
		}

		if (client->read_more == false)
		{
			printf("Sending to parser: \n");
			printf("%s\n", client->request->headers.c_str());
			printf("%s\n", client->request->body.c_str());
			//client->request->parser();
			client->CreateResponse();
			client->response->ConstructResponse();

			//Response is ready to be sent, so we monitor client->stream_socket for writing only
			pthread_mutex_lock(&thread_info->epoll_fd_mutex);
			thread_info->event.data.fd = client->stream_socket;
			thread_info->event.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
			epoll_ctl(*thread_info->epoll_fd, EPOLL_CTL_MOD, client->stream_socket, &thread_info->event);
			client->response_ready = true;
			pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
			return ;
		}
	}

	//The request is incomplete, we monitor the connection again to read the rest
	printf("Incomplete request \n");
	pthread_mutex_lock(&thread_info->epoll_fd_mutex);
	thread_info->event.data.fd = client->stream_socket;
	thread_info->event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	epoll_ctl(*thread_info->epoll_fd,
			EPOLL_CTL_MOD,
			client->stream_socket,
			&thread_info->event);
	pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
}

void	thread_send_routine(Client *client, t_thread_info *thread_info)
{	
	printf("ThreadsPool: send routine\n");

	//Sending the reponse to the client
	send(client->stream_socket,
			client->response->raw_response.c_str(),
			client->response->raw_response.size(),
			0);

	delete client->request;
	client->request = NULL;
	delete client->response;
	client->response = NULL;
	client->response_ready = false;
	client->request_buffer.clear();

	//Response was sent, so we monitor client->stream_socket for receiving a new request
	pthread_mutex_lock(&thread_info->epoll_fd_mutex);
	thread_info->event.data.fd = client->stream_socket;
	thread_info->event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	epoll_ctl(*thread_info->epoll_fd,
			EPOLL_CTL_MOD,
			client->stream_socket,
			&thread_info->event);
	pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
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
