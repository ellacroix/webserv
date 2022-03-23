#include "Port.hpp"
#include "Virtual_Server.hpp"
#include "Client.hpp"

#include <list>
#include <pthread.h>
#include <sys/select.h>

#define THREADS 8

int main()
{
	Port *Port1 = new Port(8000);
	Port *Port2 = new Port(8001);

	std::list<Port *> Ports;
	Ports.push_back(Port1);
	Ports.push_back(Port2);

	Virtual_Server *VS1 = new Virtual_Server("Alfred");
	Virtual_Server *VS2 = new Virtual_Server("Zac");
	Virtual_Server *VS3 = new Virtual_Server("Joe");

	Port1->Virtual_Servers.push_back(VS1);
	Port1->Virtual_Servers.push_back(VS2);
	Port2->Virtual_Servers.push_back(VS3);

	fd_set	master_reading_set, work_reading_set, master_writing_set, work_writing_set;
	FD_ZERO(&master_reading_set);
	FD_ZERO(&master_writing_set);

	//CREATE THREAD_POOL

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

		for (std::iterator<list> it_p = Ports.begin(); it_p != Ports.end(); it_p++)
		{
			if (FD_ISSET(it_p->listen_socket, &work_reading_set))
			{
				int connection = 0;
				while (connection != -1)
				{
					printf("Accepting new connection\n");
					connection = accept(listen_socket, NULL, NULL);
					if (connection < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("accept() failed");
							end_server = true;
						}
						break;
					}
					Client *newClient = new Client(connection);
					it_p->Clients[connection] = newClient;
				}
			}

		}
	}
}