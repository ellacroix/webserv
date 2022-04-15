#include "webserv.hpp"

#include "ConfigParser.hpp"
#include "Port.hpp"

int		startAllPorts(ConfigParser &config, struct epoll_event event,
		int epoll_fd)
{
	std::list<Port*>::iterator  it;
	std::list<Port*>::iterator  ite;
	it = config.getports_list().begin();
	ite = config.getports_list().end();
	while (it != ite)
	{
		(*it)->start();
		event.data.fd = (*it)->listen_socket;
		event.events = EPOLLIN;
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, (*it)->listen_socket, &event);
		it++;
	}
	return (SUCCESS);
}

int		acceptIncomingConnections(Port *current_port,
		struct epoll_event &event,
		int epoll_fd,
		t_thread_info *thread_info,
		int	*current_connections)
{
	//Loop to accept all connections on the backlog
	int connection = 0;
	while (connection != -1)
	{
		connection = accept(current_port->listen_socket, NULL, NULL);
		if (connection < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				perror("accept() failed");
			}
			break;
		}

 		(*current_connections)++;
		if (*current_connections > MAX_CONNECTIONS)
		{
			(*current_connections)--;
			close(connection);
			continue ;
		}

		//Setting the connection socket to non blocking
		int rc = fcntl(connection, F_SETFL, O_NONBLOCK);
		if (rc < 0){
			perror("fcntl() failed");
			close(connection);
			exit(-1);
		}

		//Creating a new Client instance to represent the connection
		//and add it to this port's map.
		logger("Client " + numberToString(connection) + " accepted connection on port " + numberToString(current_port->port_number));
		Client *newClient = new Client(connection, current_port);
		current_port->_clients_map[connection] = newClient;

		// Monitor this new connection for reading.
		event.data.fd = connection;
		event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
		pthread_mutex_lock(&thread_info->epoll_fd_mutex);
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connection, &event);
		pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
	}
	return (SUCCESS);
}

void	recvClientsRequest(Port *current_port, t_thread_info *thread_info,
		t_clientMapIt it_c, int *current_connections)
{	
	int     connection;
	Client  *current_client;
	char    buffer[RECV_BUFFER_SIZE];
	int     ret;

	connection = it_c->first;
	current_client = it_c->second;

	gettimeofday(&current_client->last_activity, NULL);
	//Receiving all we can from the client
	bzero(buffer, RECV_BUFFER_SIZE);
	ret = recv(current_client->stream_socket, buffer, RECV_BUFFER_SIZE, 0);
	logger("Client " + numberToString(connection) + " received " + numberToString(ret) + " bytes");
	//logger("Client " + numberToString(connection) + " locking mutex \t- Received data from the client");
	pthread_mutex_lock(&current_client->client_mutex);
	//logger("Client " + numberToString(connection) + " locked mutex \t\t- Received data from the client");
	if (ret <= 0)
	{
		//Client disconnected itself
		current_client->connected = false;
		current_port->_clients_map.erase(current_client->stream_socket);
		pthread_mutex_unlock(&current_client->client_mutex);
		//logger("Client " + numberToString(connection) + " unlocked mutex \t- Client disconnected by writing 0 bytes");
		delete current_client;
		(*current_connections)--;
	}
	else
	{
		//Add the client to the wait_queue
		current_client->request_buffer.append(buffer, ret);
		pthread_mutex_lock(&thread_info->queue_mutex);
		thread_info->queue->push_back(current_client);
		pthread_cond_signal(&thread_info->condition_var);
		logger("Client " + numberToString(connection) + " added to the queue for recv routine");
		//logger("Dequeue size = " + numberToString(thread_info->queue->size()));
		pthread_mutex_unlock(&thread_info->queue_mutex);
		pthread_mutex_unlock(&current_client->client_mutex);
		//logger("Client " + numberToString(connection) + " unlocked mutex \t- Received data to analyze");
	}
}

void	sendClientResponse(t_thread_info *thread_info,
		t_clientMapIt it_c)
{	
	int     connection;
	Client  *current_client;

	connection = it_c->first;
	current_client = it_c->second;

	//logger("Client " + numberToString(connection) + " locking mutex \t- Client ready to be written to");
	pthread_mutex_lock(&current_client->client_mutex);
	//logger("Client " + numberToString(connection) + " locked mutex \t\t- Client ready to be written to");
	pthread_mutex_lock(&thread_info->queue_mutex);
	thread_info->queue->push_back(current_client);
	pthread_cond_signal(&thread_info->condition_var);
	logger("Client " + numberToString(connection) + " added to the queue for send routine");
	//logger("Dequeue size = " + numberToString(thread_info->queue->size()));
	pthread_mutex_unlock(&thread_info->queue_mutex);
	pthread_mutex_unlock(&current_client->client_mutex);
	//logger("Client " + numberToString(connection) + " unlocked mutex \t- Client ready to be written to");
}

int	disconnectTimeout408(std::list<Port*> ports_list, t_thread_info *thread_info, int *current_connections)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	//logger("Checking for timeouts");
	for (std::list<Port*>::iterator it_p = ports_list.begin(); it_p != ports_list.end(); it_p++)
	{
		Port *current_port = *it_p;

		for (std::map<int, Client*>::iterator it_c = current_port->_clients_map.begin(), next_it_c = it_c; it_c != current_port->_clients_map.end(); it_c = next_it_c)
		{
			Client *current_client = it_c->second;
			next_it_c++;

			//logger("Client " + numberToString(current_client->stream_socket) + " locking mutex \t- Checking timeout");
			pthread_mutex_lock(&current_client->client_mutex);
			//logger("Client " + numberToString(current_client->stream_socket) + " locked mutex \t\t- Checking timeout");

			int result = current_time.tv_sec - current_client->last_activity.tv_sec;

			if (current_client->connected == false)
			{
				logger("Client " + numberToString(current_client->stream_socket) + " disconnected");
				current_port->_clients_map.erase(current_client->stream_socket);
				pthread_mutex_unlock(&current_client->client_mutex);
				//logger("Client " + numberToString(current_client->stream_socket) + " unlocked mutex \t- Disconnected client");
				delete current_client;
				(*current_connections)--;
				continue;
			}

			else if (result > TIMEOUT && current_client->response_ready == false)
			{
				logger("Client " + numberToString(current_client->stream_socket) + " exceeded timeout");
				current_client->status_code = 408;
				current_client->response_ready = true;
				current_client->createResponse();
				current_client->response->ConstructResponse();

				monitorForWriting(current_client, thread_info);
			}
			pthread_mutex_unlock(&current_client->client_mutex);
			//logger("Client " + numberToString(current_client->stream_socket) + " unlocked mutex \t- Checked timeout");
		}
	}
	return 0;
}

void	cleanShutDown(pthread_t *thread_pool, t_thread_info *thread_info)
{
	Client	*killer_client = new Client();
	
	logger("\n--------------------------------END SERVER---------------------------");
	for (int i = 0; i < THREADS; i++)
	{
		pthread_mutex_lock(&thread_info->queue_mutex);
		thread_info->queue->push_back(killer_client);
		pthread_cond_signal(&thread_info->condition_var);
		pthread_mutex_unlock(&thread_info->queue_mutex);
	}

	for (int i = 0; i < THREADS; i++)
	{
		pthread_join(thread_pool[i], NULL);
	}

 	pthread_cond_destroy(&thread_info->condition_var);
	pthread_mutex_destroy(&thread_info->epoll_fd_mutex);
	pthread_mutex_destroy(&thread_info->queue_mutex);
	delete killer_client;

	delete thread_info->queue;
	delete thread_info;
}