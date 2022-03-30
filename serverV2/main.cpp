#include "webserv.hpp"
#include "Port.hpp"
#include "VirtualServer.hpp"
#include "Client.hpp"
#include "ConfigParser.hpp"

#include <list>
#include <deque>
#include <pthread.h>
#include <sys/select.h>

#define THREADS 8

/*
void	*thread_loop(void* arg)
{
//	std::deque<Client*> *queue = (std::deque<Client*>*)arg;
//	Client *currentClient;
	
//	currentClient = (Client*)queue->pop_front();
}
*/

int main(int argc, char *argv[])
{
	ConfigParser	config;
	if (argc != 2)
		exit (1);
//	std::list<Port *> portsList = config.parse(argv[1]);
	config.parse(argv[1]);
	config.startAllSockets();

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


	fd_set	master_reading_set, work_reading_set, master_writing_set, work_writing_set;
	FD_ZERO(&master_reading_set);
	FD_ZERO(&master_writing_set);

	//CREATE THREAD_POOL
	std::deque<Client*> queue;

	Port1->start();
	FD_SET(Port1->listen_socket, &master_reading_set);
	Port2->start();
	FD_SET(Port2->listen_socket, &master_reading_set);

	int max_sd = Port2->listen_socket;
	int	active = 0;
	struct timeval		timeout;
	timeout.tv_sec  = 60;
	timeout.tv_usec = 0;
	while (1)
	{
		work_reading_set = master_reading_set;
		work_writing_set = master_writing_set;

		printf("\nWaiting on select()\n");
		active = select(max_sd + 1, &work_reading_set, &work_writing_set, NULL, &timeout);
		if (active < 0){
			perror("select() failed");
			break;
		}
		if (active == 0){
			printf("select() timed out.  End program.\n");
			break;
		}

		for (std::list<Port*>::iterator it_p = Ports.begin(); it_p != Ports.end(); it_p++)
		{
			Port *current_port = *it_p;

			if (FD_ISSET(current_port->listen_socket, &work_reading_set))
			{
				int connection = 0;
				while (connection != -1)
				{
					printf("Accepting new connection\n");
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
					Client *newClient = new Client(connection);
					current_port->Clients[connection] = newClient;
					FD_SET(connection, &master_reading_set);
					max_sd = connection;
				}
			}

			//Port::Clients current_clients = current_port->Clients;
			std::map<int, Client*> current_clients = current_port->Clients;
			for (std::map<int, Client*>::iterator it_c = current_clients.begin(); it_c != current_clients.end(); it_c++)
			{
				int connection = it_c->first;
				Client *current_client = it_c->second;

				if (FD_ISSET(connection, &work_reading_set))
				{
					queue.push_back(current_client);
				}
			}
		}
	}
*/
}
