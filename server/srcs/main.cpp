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
#include <sys/types.h>

#include "webserv.hpp"

#define MAX_EVENTS 20
#define BUFFER_SIZE 50000

bool RUNNING = true;

static void shutdownWebserv(int sig_int) {
  (void)sig_int;
  RUNNING = false;
}

int main(int argc, char *argv[])
{
	unlink("log.log");
	logger("Start with PID: " + numberToString(syscall(__NR_gettid)));
	
	signal(SIGINT, shutdownWebserv);
    signal(SIGQUIT, shutdownWebserv);
	signal(SIGPIPE, SIG_IGN);
	
	if (argc > 3)
		return (-1);
	ConfigParser	config;
	if (argc == 1)
	{
		char default_path[17] = "confs/nginx.conf";
		config.parse(default_path);
	}
	else
		config.parse(argv[1]);

	
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
	
	int current_connections = 0;
	logger("\n-------------------------------START SERVER--------------------------");
	while (RUNNING)
	{
		int new_events = epoll_wait(epoll_fd, events, MAX_EVENTS, 70000);
		logger("epoll_wait() activated by " + numberToString(new_events) + " file descriptors");
		if (new_events < 0){
			perror("epoll_wait() failed");
			break;
		}
		if (new_events == 0)
			logger("epoll_wait() timed out. Checking clients timeout.");
		disconnectTimeout408(config.getports_list(), thread_info, &current_connections);

		//LOOP TO CHECK ALL ACTIVATED FD
		for (int i = 0; i < new_events; i++)
		{
			int event_fd = events[i].data.fd;

			for (t_portListIt it_p = config.getports_list().begin() ;
					it_p != config.getports_list().end() ;
					it_p++)
			{
				Port *current_port = *it_p;

				//Check if the listen_socket of the current port has activated,
				//meaning we have connections to accept()
				if (current_port->listen_socket == event_fd)
				{
					acceptIncomingConnections(current_port,
							event, epoll_fd, thread_info, &current_connections);
					continue;
				}
				//Check if find() found the event_fd in this clients map,
				//meaning there is work to do in it.
				t_clientMapIt	it_c;
				it_c = current_port->_clients_map.find(event_fd);
				if (it_c != current_port->_clients_map.end())
				{
					//printf("HERE: %d ---- %d\n", events[i].events & EPOLLOUT, events[i].events & EPOLLIN);
					if (events[i].events & EPOLLOUT)
						sendClientResponse(thread_info, it_c);
					else if (events[i].events & EPOLLIN)
						recvClientsRequest(current_port, thread_info, it_c, &current_connections);
				}
			}
		}
	}
	printf("Ending server\n");
	cleanShutDown(thread_pool, thread_info);
	printf("End of server\n");
	return (SUCCESS);
}
