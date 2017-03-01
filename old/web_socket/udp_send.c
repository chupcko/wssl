#include "main.h"

void udp_sent(char* remote_ip, int remote_port, char* message)
{
  int socket_descriptor;
  struct sockaddr_in server_address;

  socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0);
  if(socket_descriptor < 0)
    error("Cannot socket(%d %s)", errno, strerror(errno));

  bzero(&server_address, (size_t)sizeof(struct sockaddr_in));
  server_address.sin_family = PF_INET;
  if(inet_pton(PF_INET, remote_ip, (void *)&server_address.sin_addr) <= 0)
    error("Cannot inet_pton(%d %s)", errno, strerror(errno));
  server_address.sin_port = htons((uint16_t)remote_port);
  if(sendto(socket_descriptor, message, strlen(message), 0, (struct sockaddr *)&server_address, (socklen_t)sizeof(struct sockaddr_in)) < 0)
    error("Cannot sendto(%d %s)", errno, strerror(errno));
  close(socket_descriptor);
}
