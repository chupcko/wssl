#include "main.h"

int udp_server_init(int server_port)
{
  int server_socket_fd;
  struct sockaddr_in server_address;

  server_socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
  if(server_socket_fd < 0)
    error("Cannot socket(%d %s)", errno, strerror(errno));
  bzero(&server_address, (size_t)sizeof(struct sockaddr_in));
  server_address.sin_family = PF_INET;
  server_address.sin_addr.s_addr = htonl((uint32_t)INADDR_ANY);
  server_address.sin_port = htons((uint16_t)server_port);
  if(bind(server_socket_fd, (struct sockaddr *)&server_address, (socklen_t)sizeof(struct sockaddr_in)) < 0)
    error("Cannot bind(%d %s)", errno, strerror(errno));

  debug("Added udp server on server_port=%d", server_port);
  return server_socket_fd;
}
