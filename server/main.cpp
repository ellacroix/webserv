#include "Port.hpp"
#include "Virtual_Server.hpp"
#include "Client.hpp"

#include <list>
#include <deque>
#include <iostream>
#include <pthread.h>
#include <sys/epoll.h>

#define MAX_EVENTS 20
#define THREADS 1

typedef struct	s_thread_info
{
	std::deque<Client*> *queue;
	pthread_mutex_t		queue_mutex;

	pthread_cond_t		condition_var;

	pthread_mutex_t		epoll_fd_mutex;
	int					*epoll_fd;
	struct epoll_event	event;

}				t_thread_info;

void	thread_recv_routine(Client *client, t_thread_info *thread_info)
{	
	char	buffer[BUFFER_SIZE];
	int		ret;
	printf("ThreadsPool: recv routine\n");
	
	//Receiving all we can from the client
	bzero(buffer, BUFFER_SIZE);
	ret = recv(client->stream_socket, buffer, BUFFER_SIZE, 0);
	if (ret == 0)
	{
		//Client disconnected itself
		client->connected = false;
		//The main has to disconnect and erase the client.
 		client->parent_port->Clients.erase(client->stream_socket);
		delete client;
		return ;
	}
	client->request_buffer.append(buffer);

	//Simulating the processing of a big request
	//sleep(5);

	//Checking if client->request_buffer is a complete request
	if (client->CheckCompleteRequest() == true)
	{
		client->CreateRequest();
		client->request->parser();
		
		//Response is ready to be sent, so we monitor client->stream_socket for writing only
		pthread_mutex_lock(&thread_info->epoll_fd_mutex);
		thread_info->event.data.fd = client->stream_socket;
		thread_info->event.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
		epoll_ctl(*thread_info->epoll_fd, EPOLL_CTL_MOD, client->stream_socket, &thread_info->event);
		client->response_ready = true;
		pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
	}
	else
	{
		//The request is incomplete, we monitor the connection again to read the rest
		pthread_mutex_lock(&thread_info->epoll_fd_mutex);
		thread_info->event.data.fd = client->stream_socket;
		thread_info->event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
		epoll_ctl(*thread_info->epoll_fd, EPOLL_CTL_MOD, client->stream_socket, &thread_info->event);
		pthread_mutex_unlock(&thread_info->epoll_fd_mutex);
	}
}

void	thread_send_routine(Client *client, t_thread_info *thread_info)
{	
	printf("ThreadsPool: send routine\n");
	
	//Sending the reponse to the client
	send(client->stream_socket, client->request_buffer.c_str(), client->request_buffer.size(), 0);
	
	//Response was sent, so we monitor client->stream_socket for receiving a new request
	pthread_mutex_lock(&thread_info->epoll_fd_mutex);
	thread_info->event.data.fd = client->stream_socket;
	thread_info->event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	epoll_ctl(*thread_info->epoll_fd, EPOLL_CTL_MOD, client->stream_socket, &thread_info->event);
	client->response_ready = false;
	client->request_buffer.clear();
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
			usleep(50);
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

int main()
{
	Port *Port1 = new Port(8000);
	Port *Port2 = new Port(8001);

	std::list<Port*> Ports; //= parse();
	Ports.push_back(Port1);
	Ports.push_back(Port2);

	Virtual_Server *VS1 = new Virtual_Server("Alfred");
	Virtual_Server *VS2 = new Virtual_Server("Zac");
	Virtual_Server *VS3 = new Virtual_Server("Joe");

	Port1->Virtual_Servers.push_back(VS1);
	Port1->Virtual_Servers.push_back(VS2);
	Port2->Virtual_Servers.push_back(VS3);

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
	Port1->start();
	event.data.fd = Port1->listen_socket;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Port1->listen_socket, &event);
	Port2->start();
	event.data.fd = Port2->listen_socket;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Port2->listen_socket, &event);

	while (1)
	{
		printf("\nMainProcess: Waiting on epoll_wait()\n");
		int new_events = epoll_wait(epoll_fd, events, MAX_EVENTS, 60000);
		printf("MainProcess: epoll_wait() activated by %d file descriptors\n", new_events);
		if (new_events < 0){
			perror("epoll_wait() failed");
			break;
		}
		if (new_events == 0){
			printf("epoll_wait() timed out.  End program.\n");
			break;
		}

		for (int i = 0; i < new_events; i++)
		{
			int event_fd = events[i].data.fd;

			for (std::list<Port*>::iterator it_p = Ports.begin(); it_p != Ports.end(); it_p++)
			{
				Port *current_port = *it_p;
				std::map<int, Client*>::iterator it_c = current_port->Clients.find(event_fd);

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
				}
				
				//Check if find() found the event_fd in this port's map, meaning there is work to do in it.
				else if (it_c != current_port->Clients.end())
				{
					int connection = it_c->first;
					Client *current_client = it_c->second;

					pthread_mutex_lock(&thread_info->queue_mutex);
					thread_info->queue->push_back(current_client);
					pthread_cond_signal(&thread_info->condition_var);
					printf("MainProcess: added client %d to the queue\n", connection);
					pthread_mutex_unlock(&thread_info->queue_mutex);
				}
			}
		}
	}
	return 0;
}
