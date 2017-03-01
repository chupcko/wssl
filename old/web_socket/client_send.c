#include "main.h"

void client_send(int client_socket_fd, uint8_t* buffer, int buffer_length)
{
  int totaly_sent;
  int send_size;

  totaly_sent = 0;
  do
  {
    send_size = send(client_socket_fd, &buffer[totaly_sent], buffer_length-totaly_sent, 0);
    if(send_size < 0)
      error("Cannot send(%d %s)", errno, strerror(errno));
    totaly_sent += send_size;
  }
  while(totaly_sent < buffer_length);
}
