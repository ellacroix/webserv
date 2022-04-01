#include "Port.hpp"
#include "VirtualServer.hpp"
#include "Client.hpp"
#include "ConfigParser.hpp"

#include <list>
#include <deque>
#include <iostream>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <time.h>

#include "webserv.hpp"

#define MAX_EVENTS 20
#define THREADS 2
#define BUFFER_SIZE 50000
#define TIMEOUT	10

int	DisconnectTimeout408(std::list<Port*> PortsList)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	printf("MainProcess: Checking for timeouts\n");
	for (std::list<Port*>::iterator it_p = PortsList.begin(); it_p != PortsList.end(); it_p++)
	{
		Port *current_port = *it_p;

		for (std::map<int, Client*>::iterator it_c = current_port->Clients.begin(), next_it_c = it_c; it_c != current_port->Clients.end(); it_c = next_it_c)
		{
			Client *current_client = it_c->second;
			next_it_c++;

			pthread_mutex_lock(&current_client->client_mutex);
			int result = current_time.tv_sec - current_client->last_activity.tv_sec;
			if (result > TIMEOUT)
			{
				current_client->connected = false;
				current_port->Clients.erase(current_client->stream_socket);
				pthread_mutex_unlock(&current_client->client_mutex);
				delete current_client;
				continue;
			}
			pthread_mutex_unlock(&current_client->client_mutex);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	/*
	Port *Port1 = new Port(8000);
	Port *Port2 = new Port(8001);

	std::list<Port*> Ports; //= parse();
	Ports.push_back(Port1);
	Ports.push_back(Port2);

	VirtualServer *VS1 = new VirtualServer("Alfred");
	VirtualServer *VS2 = new VirtualServer("Zac");
	VirtualServer *VS3 = new VirtualServer("Joe");

	Port1->VirtualServers.push_back(VS1);
	Port1->VirtualServers.push_back(VS2);
	Port2->VirtualServers.push_back(VS3);
	*/
	if (argc != 2)
		return (-1);
	ConfigParser	config;
	config.parse(argv[1]);


	//EPOLL
	struct epoll_event event;
	struct epoll_event events[MAX_EVENTS];
	int epoll_fd = epoll_create(1);


	//CREATE AND LAUNCH THREAD_POOL
	t_thread_info *thread_info = new t_thread_info();
	
	thread_info->queue = new std::deque<Client*>();
	pthread_mutex_init(&thread_info->queue_mutex, NULL);
	pthread_cond_init(&thread_info->condition_var, NULL);
	thread_info->epoll_fd = &epoll_fd;
	pthread_mutex_init(&thread_info->epoll_fd_mutex, NULL);
	
	pthread_t	thread_pool[THREADS];
	for(int i = 0; i < THREADS; i++)
	{
		pthread_create(&thread_pool[i], NULL, thread_loop, thread_info);
		usleep(500);
	}
	

	//SETUP AND LAUNCH LISTEN SOCKETS ON ALL PORTS
	/*
	Port1->start();
	event.data.fd = Port1->listen_socket;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Port1->listen_socket, &event);
	Port2->start();
	event.data.fd = Port2->listen_socket;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Port2->listen_socket, &event);
	*/
	std::list<Port*>::iterator	it;
	std::list<Port*>::iterator	ite;
	it = config.getPortsList().begin();
	ite = config.getPortsList().end();
	while (it != ite)
	{
		//Port1->start();
		(*it)->start();
		//event.data.fd = Port1->listen_socket;
		event.data.fd = (*it)->listen_socket;
		event.events = EPOLLIN | EPOLLET;
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, (*it)->listen_socket, &event);
		it++;
	}

	while (1)
	{
		printf("\nMainProcess: Waiting on epoll_wait()\n");
		int new_events = epoll_wait(epoll_fd, events, MAX_EVENTS, 5000);

		printf("MainProcess: epoll_wait() activated by %d file descriptors\n", new_events);
		if (new_events < 0){
			perror("epoll_wait() failed");
			break;
		}
		if (new_events == 0){
			printf("MainProcess: epoll_wait() timed out.  Checking for clients timeout.\n");
		}

		DisconnectTimeout408(config.getPortsList());

		//LOOP TO CHECK ALL ACTIVATED FD
		for (int i = 0; i < new_events; i++)
		{
			int event_fd = events[i].data.fd;

			for (std::list<Port*>::iterator it_p = config.getPortsList().begin(); it_p != config.getPortsList().end(); it_p++)
			{
				Port *current_port = *it_p;

				//Check if the listen_socket of the current port has activated, meaning we have connections to accept()
				if (current_port->listen_socket == event_fd)
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
						//Setting the connection socket to non blocking
						int rc = fcntl(connection, F_SETFL, O_NONBLOCK);
						if (rc < 0){
							perror("fcntl() failed");
							close(connection);
							exit(-1);
						}
						
						//Creating a new Client instance to represent the connection and add it to this port's map.
						printf("MainProcess: Accepted new connection on port:%d\n", current_port->port_number);
						Client *newClient = new Client(connection, current_port);
						current_port->Clients[connection] = newClient;
						
						// Monitor this new connection for reading.
						event.data.fd = connection;
						event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
						pthread_mutex_lock(&thread_info->epoll_fd_mutex);
						epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connection, &event);
						pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
					}
					continue;
				}
				
				//Check if find() found the event_fd in this port's map, meaning there is work to do in it.
				std::map<int, Client*>::iterator it_c = current_port->Clients.find(event_fd);
				if (it_c != current_port->Clients.end())
				{
					int connection = it_c->first;
					Client *current_client = it_c->second;
					char	buffer[BUFFER_SIZE];
					int		ret;

					gettimeofday(&current_client->last_activity, NULL);
					//Receiving all we can from the client
					bzero(buffer, BUFFER_SIZE);
					ret = recv(current_client->stream_socket, buffer, BUFFER_SIZE, 0);

					pthread_mutex_lock(&current_client->client_mutex);
					if (ret == 0)
					{
						//Client disconnected itself
						current_client->connected = false;
						current_port->Clients.erase(current_client->stream_socket);
						pthread_mutex_unlock(&current_client->client_mutex);
						delete current_client;
					}
					
					else
					{
						current_client->request_buffer.append(buffer, ret);
						pthread_mutex_lock(&thread_info->queue_mutex);
						thread_info->queue->push_back(current_client);
						pthread_cond_signal(&thread_info->condition_var);
						printf("MainProcess: added client %d to the queue\n", connection);
						pthread_mutex_unlock(&thread_info->queue_mutex);
						pthread_mutex_unlock(&current_client->client_mutex);
					}
				}
			}
		}
	}
	return 0;
}
