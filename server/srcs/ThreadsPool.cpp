#include "webserv.hpp"

int		s_thread_info::start(int *epoll_fd, pthread_t *thread_pool)
{
    this->queue = new std::deque<Client*>();
    pthread_mutex_init(&this->queue_mutex, NULL);
    pthread_cond_init(&this->condition_var, NULL);
    this->epoll_fd = epoll_fd;
    pthread_mutex_init(&this->epoll_fd_mutex, NULL);

//    pthread_t   thread_pool[THREADS];
    for(int i = 0; i < THREADS; i++)
    {
        pthread_create(&thread_pool[i], NULL, threadLoop, this);
        usleep(500);
    }
	return (SUCCESS);
}
