#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 5000

int main()
{
  int running = 1, event_count, i;
  size_t bytes_read;
  char read_buffer[BUFFER_SIZE + 1];
  struct epoll_event event, events[MAX_EVENTS];
  int epoll_fd = epoll_create(10000);
 
  event.events = EPOLLIN;
  event.data.fd = 0;
 
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event);
 
  while(running)
  {
    printf("\nPolling for input...\n");
    event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
    printf("%d ready events\n", event_count);
    for(i = 0; i < event_count; i++)
    {
      printf("Reading file descriptor '%d' -- ", events[i].data.fd);
      bytes_read = read(events[i].data.fd, read_buffer, BUFFER_SIZE);
      printf("%zd bytes read.\n", bytes_read);
      read_buffer[bytes_read] = '\0';
      printf("Read '%s'\n", read_buffer);
 
      if(!strncmp(read_buffer, "stop\n", 5))
        running = 0;
    }
  }
 
  if(close(epoll_fd))
  {
    fprintf(stderr, "Failed to close epoll file descriptor\n");
    return 1;
  }
  return 0;
}