#include "main.h"

int udp_recv(int socket_fd, uint8_t* buffer, int buffer_size, char* client_ip, int client_ip_size, int* client_port)
{
  struct sockaddr_in client_address;
  socklen_t client_address_length;
  int udp_size;

  bzero(&client_address, (size_t)sizeof(struct sockaddr_in));
  client_address_length = (socklen_t)sizeof(struct sockaddr_in);
  udp_size = recvfrom(socket_fd, (void *)buffer, (size_t)buffer_size-1, 0, (struct sockaddr *)&client_address, &client_address_length);
  if(udp_size < 0)
    error("Cannot recvfrom(%d %s)", errno, strerror(errno));

  buffer[udp_size] = '\0';
  inet_ntop(AF_INET, (void*)&client_address.sin_addr, client_ip, client_ip_size);
  *client_port = (int)ntohs(client_address.sin_port);

  return udp_size;
}
