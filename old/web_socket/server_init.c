#include "main.h"

int server_init(int server_port)
{
  int socket_fd;
  int socket_reusable;
  struct sockaddr_in server_address;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd < 0)
    error("Cannot socket(%d %s)", errno, strerror(errno));
  socket_reusable = 1;
  if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_reusable, sizeof socket_reusable) < 0)
    error("Cannot setsockopt(%d %s)", errno, strerror(errno));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(server_port);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(socket_fd, (struct sockaddr*)&server_address, sizeof(struct sockaddr)) < 0)
    error("Cannot bind(%d %s)", errno, strerror(errno));

  if(listen(socket_fd, SERVER_BACKLOG) < 0)
    error("Cannot listen(%d %s)", errno, strerror(errno));

  debug("Added server on server_port=%d", server_port);

  return socket_fd;
}
