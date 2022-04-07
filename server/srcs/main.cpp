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
#define BUFFER_SIZE 50000

int main(int argc, char *argv[])
{
	if (argc != 2)
		return (-1);
	ConfigParser	config;
	config.parse(argv[1]);

	/*
	//EPOLL
	struct epoll_event event;
	struct epoll_event events[MAX_EVENTS];
	int epoll_fd = epoll_create(1);


	//CREATE AND LAUNCH THREAD_POOL
	t_thread_info *thread_info = new t_thread_info();
	pthread_t   thread_pool[THREADS];
	if (thread_info->start(&epoll_fd, thread_pool) != SUCCESS)
		return (FAILURE);
	//START LISTENING ON PORTS
	if (startAllPorts(config, event, epoll_fd) != SUCCESS)
		return (FAILURE);
	while (1)
	{
		printf("\nMainProcess: Waiting on epoll_wait()\n");
		int new_events = epoll_wait(epoll_fd, events, MAX_EVENTS, 600000);

		printf("MainProcess: epoll_wait() activated by %d file descriptors\n",
				new_events);
		if (new_events < 0){
			perror("epoll_wait() failed");
			break;
		}
		if (new_events == 0)
			printf("MainProcess: epoll_wait() timed out. Checking clients timeout.\n");
		DisconnectTimeout408(config.getPortsList());

		//LOOP TO CHECK ALL ACTIVATED FD
		for (int i = 0; i < new_events; i++)
		{
			int event_fd = events[i].data.fd;

			for (t_portListIt it_p = config.getPortsList().begin() ;
					it_p != config.getPortsList().end() ;
					it_p++)
			{
				Port *current_port = *it_p;

				//Check if the listen_socket of the current port has activated,
				//meaning we have connections to accept()
				if (current_port->listen_socket == event_fd)
				{
					acceptIncomingConnections(current_port,
							event, epoll_fd, thread_info);
					continue;
				}
				//Check if find() found the event_fd in this clients map,
				//meaning there is work to do in it.
				t_clientMapIt	it_c;
				it_c = current_port->_clientsMap.find(event_fd);
				if (it_c != current_port->_clientsMap.end())
				{
					if (events[i].events & EPOLLOUT)
						sendClientResponse(thread_info, it_c);
					else
						recvClientsRequest(current_port, thread_info, it_c);
				}
			}
		}
	}
	*/
	return (SUCCESS);
}
