#include "Port.hpp"
#include "Virtual_Server.hpp"
#include "Client.hpp"

#include <list>
#include <deque>
#include <pthread.h>
#include <sys/select.h>

#define THREADS 2

typedef struct	s_thread_info
{
	std::deque<Client*> *queue;
	pthread_mutex_t		queue_mutex;

	fd_set				*master_reading_set;
	pthread_mutex_t		master_reading_set_mutex;

	fd_set				*master_writing_set;
	pthread_mutex_t		master_writing_set_mutex;

	pthread_cond_t		condition_var;

	int					pipetrick_fd[2];
}				t_thread_info;


void	thread_recv_routine(Client *client, t_thread_info *thread_info)
{	
	(void)client;
	char *buffer[BUFFER_SIZE];
	printf("recv routine\n");
	
	//Receiving all we can from the client
	recv(client->stream_socket, buffer, BUFFER_SIZE, 0);
	sleep(20);

	//Re-adding the client to the reading_set so it is monitored again
	pthread_mutex_lock(&thread_info->master_reading_set_mutex);
	FD_SET(client->stream_socket, thread_info->master_reading_set);
	pthread_mutex_unlock(&thread_info->master_reading_set_mutex);
	write(thread_info->pipetrick_fd[1], "a", 1);

	//client->response = true;
}

void	thread_send_routine(Client *client, t_thread_info *thread_info)
{
	(void)client;
	(void)thread_info;

	printf("send routine\n");
	//client->response = false;
}

void	*thread_loop(void* arg)
{
	t_thread_info *thread_info = (t_thread_info*)arg;
	Client *currentClient;
	
	printf("Thread launched\n");
	
	while(true)
	{
		pthread_mutex_lock(&thread_info->queue_mutex);
		if (thread_info->queue->empty() == true)
		{
			printf("Waiting on conditional wait\n");
			pthread_cond_wait(&thread_info->condition_var, &thread_info->queue_mutex);
			currentClient = thread_info->queue->front();
		}
		else
			currentClient = thread_info->queue->front();
		printf("Grabbed a task\n");
		thread_info->queue->pop_front();
		pthread_mutex_unlock(&thread_info->queue_mutex);

		pthread_mutex_lock(&currentClient->client_mutex);
		if (currentClient->response == false)
			thread_recv_routine(currentClient, thread_info);
		else
			thread_send_routine(currentClient, thread_info);
		pthread_mutex_unlock(&currentClient->client_mutex);
	}

	while(true)
	{
		sleep(10);
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

	fd_set	master_reading_set, work_reading_set, master_writing_set, work_writing_set;
	FD_ZERO(&master_reading_set);
	FD_ZERO(&master_writing_set);

	//CREATE THREAD_POOL
	t_thread_info *thread_info = new t_thread_info();
	
	thread_info->queue = new std::deque<Client*>();
	thread_info->master_reading_set = &master_reading_set;
	thread_info->master_writing_set = &master_writing_set;
	pthread_mutex_init(&thread_info->queue_mutex, NULL);
	pthread_mutex_init(&thread_info->master_writing_set_mutex, NULL);
	pthread_cond_init(&thread_info->condition_var, NULL);
	
	//PIPE-TRICK
	pipe(thread_info->pipetrick_fd);
	fcntl(thread_info->pipetrick_fd[0], F_SETFL, O_NONBLOCK);
	fcntl(thread_info->pipetrick_fd[1], F_SETFL, O_NONBLOCK);
	FD_SET(thread_info->pipetrick_fd[0], &master_reading_set);

	pthread_t	thread_pool[THREADS];
	for(int i = 0; i < THREADS; i++)
	{
		pthread_create(&thread_pool[i], NULL, thread_loop, thread_info);
		usleep(500);
	}
	

	//SETUP AND LAUNCH LISTEN SOCKETS ON ALL PORTS
	Port1->start();
	FD_SET(Port1->listen_socket, &master_reading_set);
	Port2->start();
	FD_SET(Port2->listen_socket, &master_reading_set);

	int max_sd = Port2->listen_socket;
	struct timeval		timeout;
	timeout.tv_sec  = 60;
	timeout.tv_usec = 0;
	while (1)
	{
		work_reading_set = master_reading_set;
		work_writing_set = master_writing_set;

		printf("\nWaiting on select()\n");
		int active = select(max_sd + 1, &work_reading_set, &work_writing_set, NULL, &timeout);
		printf("Select activated by %d file descriptors\n", active);
		if (active < 0){
			perror("select() failed");
			break;
		}
		if (active == 0){
			printf("select() timed out.  End program.\n");
			break;
		}
		if (FD_ISSET(thread_info->pipetrick_fd[0], &work_reading_set))
		{
			char *test[BUFFER_SIZE];
			read(thread_info->pipetrick_fd[0], test, BUFFER_SIZE);
		}

		for (std::list<Port*>::iterator it_p = Ports.begin(); it_p != Ports.end(); it_p++)
		{
			Port *current_port = *it_p;

			if (FD_ISSET(current_port->listen_socket, &work_reading_set))
			{
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
					printf("Accepted new connection on port:%d\n", current_port->port_number);
					Client *newClient = new Client(connection);
					current_port->Clients[connection] = newClient;
					FD_SET(connection, &master_reading_set);
					max_sd = connection;
					active--;
				}
				if (active == 0)
					break;
			}

			for (std::map<int, Client*>::iterator it_c = current_port->Clients.begin(); it_c != current_port->Clients.end(); it_c++)
			{
				int connection = it_c->first;
				Client *current_client = it_c->second;

				if (FD_ISSET(connection, &work_reading_set))
				{
					pthread_mutex_lock(&thread_info->queue_mutex);
					//Un-monitoring this client until a thread rcv() its data
					FD_CLR(connection, &master_reading_set);
					thread_info->queue->push_back(current_client);
					pthread_cond_signal(&thread_info->condition_var);
					printf("Main added client %d to the queue\n", connection);
					pthread_mutex_unlock(&thread_info->queue_mutex);
				}
			}
		}
	}
}
