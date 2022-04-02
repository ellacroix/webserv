#include "webserv.hpp"
#include "Client.hpp"

bool	IsChunkComplete()
{
	//while ()
	return true;
}

void	thread_recv_routine(Client *client, t_thread_info *thread_info)
{	
	printf("ThreadsPool: recv routine\n");

	/* 	t_size end = client->request_buffer.find("\r\n\r\n");
		if (end != std::string::npos)
		{
		client->CreateRequest();

		std::string headers = client->request_buffer.substr(0, end);

		t_size start = headers.find_last_of("Transfer-Encoding: ");
		if (start != std::string::npos)
		{
		std::string TE_line = headers.substr(start, headers.find(start,"\r\n"));
		if (TE_line.find("chunked"))
		if (IsChunkComplete() = true);
		}
		else if((t_size start = headers.find("Content-Length: ")) != std::string::npos)
		{
		std::string CL_line = headers.substr(start, headers.find(start,"\r\n"));


		}

		if (client->read_more == false)
		{
		client->request->parser();
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
	} */

	//We dont create the Request instance until we have at least a double "CRLF"
	if (client->request == NULL)
		if (client->request_buffer.find("\r\n\r\n") != std::string::npos)
			client->CreateRequest();

	//We enter here if we received the headers or wanted to read more
	if (client->request)
	{
		client->request->status_code = client->request->parser();

		if (client->read_more == false)
		{
			printf("ThreadsPool: Reponse ready \n");
			//The request is complete and ready to be processed
			client->CreateResponse();
			client->response->ConstructResponse();

			//Response is ready to be sent,
			//so we monitor client->stream_socket for writing only
			pthread_mutex_lock(&thread_info->epoll_fd_mutex);
			thread_info->event.data.fd = client->stream_socket;
			thread_info->event.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
			epoll_ctl(*thread_info->epoll_fd,
					EPOLL_CTL_MOD,
					client->stream_socket,
					&thread_info->event);
			client->response_ready = true;
			pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
			return ;
		}
	}

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
	epoll_ctl(*thread_info->epoll_fd, EPOLL_CTL_MOD, client->stream_socket, &thread_info->event);
	pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
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
		printf("ThreadsPool: Grabbed a task\n");
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
