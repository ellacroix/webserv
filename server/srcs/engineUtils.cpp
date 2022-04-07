#include "webserv.hpp"

#include "ConfigParser.hpp"
#include "Port.hpp"

int		startAllPorts(ConfigParser &config, struct epoll_event event,
		int epoll_fd)
{
	std::list<Port*>::iterator  it;
	std::list<Port*>::iterator  ite;
	it = config.getPortsList().begin();
	ite = config.getPortsList().end();
	while (it != ite)
	{
		(*it)->start();
		event.data.fd = (*it)->listen_socket;
		event.events = EPOLLIN | EPOLLET;
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, (*it)->listen_socket, &event);
		it++;
	}
	return (SUCCESS);
}

int		acceptIncomingConnections(Port *current_port,
		struct epoll_event &event,
		int epoll_fd,
		t_thread_info *thread_info)
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
				current_port->kill_port = true;
			}
			break;
		}

/* 		current_connections++;
		if (current_connections == max_connections)
			close(connection); */

		//Setting the connection socket to non blocking
		int rc = fcntl(connection, F_SETFL, O_NONBLOCK);
		if (rc < 0){
			perror("fcntl() failed");
			close(connection);
			exit(-1);
		}

		//Creating a new Client instance to represent the connection
		//and add it to this port's map.
		printf("MainProcess: Accepted new connection on port:%d\n",
				current_port->port_number);
		Client *newClient = new Client(connection, current_port);
		current_port->_clientsMap[connection] = newClient;

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
		t_clientMapIt it_c)
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
	printf("recvClientsReq - ret = %d\n", ret);

	pthread_mutex_lock(&current_client->client_mutex);
	if (ret == 0)
	{
		//Client disconnected itself
		current_client->connected = false;
		current_port->_clientsMap.erase(current_client->stream_socket);
		pthread_mutex_unlock(&current_client->client_mutex);
		delete current_client;
	}
	else
	{
		//Add the client to the wait_queue
		current_client->request_buffer.append(buffer, ret);
		pthread_mutex_lock(&thread_info->queue_mutex);
		thread_info->queue->push_back(current_client);
		pthread_cond_signal(&thread_info->condition_var);
		printf("MainProcess: added client %d to the queue for recv routine\n", connection);
		pthread_mutex_unlock(&thread_info->queue_mutex);
		pthread_mutex_unlock(&current_client->client_mutex);
	}
}

void	sendClientResponse(t_thread_info *thread_info,
		t_clientMapIt it_c)
{	
	int     connection;
	Client  *current_client;

	connection = it_c->first;
	current_client = it_c->second;

	pthread_mutex_lock(&current_client->client_mutex);
	pthread_mutex_lock(&thread_info->queue_mutex);
	thread_info->queue->push_back(current_client);
	pthread_cond_signal(&thread_info->condition_var);
	printf("MainProcess: added client %d to the queue for send routine\n", connection);
	pthread_mutex_unlock(&thread_info->queue_mutex);
	pthread_mutex_unlock(&current_client->client_mutex);
}

int	DisconnectTimeout408(std::list<Port*> PortsList)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	printf("MainProcess: Checking for timeouts\n");
	for (std::list<Port*>::iterator it_p = PortsList.begin(); it_p != PortsList.end(); it_p++)
	{
		Port *current_port = *it_p;

		for (std::map<int, Client*>::iterator it_c = current_port->_clientsMap.begin(), next_it_c = it_c; it_c != current_port->_clientsMap.end(); it_c = next_it_c)
		{
			Client *current_client = it_c->second;
			next_it_c++;

			pthread_mutex_lock(&current_client->client_mutex);
			int result = current_time.tv_sec - current_client->last_activity.tv_sec;
			if (result > TIMEOUT)
			{
				current_client->connected = false;
				current_port->_clientsMap.erase(current_client->stream_socket);
				pthread_mutex_unlock(&current_client->client_mutex);
				delete current_client;
				continue;
			}
			pthread_mutex_unlock(&current_client->client_mutex);
		}
	}
	return 0;
}
