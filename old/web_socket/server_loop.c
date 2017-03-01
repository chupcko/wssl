#include "main.h"

void server_loop(int server_socket_fd)
{
  struct sockaddr_in client_address;
  socklen_t client_address_length;
  client_info_t client_info;
  int child_pid;

  loop
  {
    client_address_length = (socklen_t)sizeof client_address;
    client_info.socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_address, &client_address_length);
    if(client_info.socket_fd  < 0)
      error("Cannot accept(%d %s)", errno, strerror(errno));

    inet_ntop(AF_INET, (void*)&client_address.sin_addr, client_info.ip, IP_SIZE);
    client_info.port = ntohs(client_address.sin_port);
    debug("New connection from %s:%d", client_info.ip, client_info.port);
    if(Callback_connect != NULL)
      (*Callback_connect)(&client_info, Callback_extra_data);

    child_pid = fork();
    if(child_pid < 0)
      error("Cannot fork(%d %s)", errno, strerror(errno));
    if(child_pid == 0)
    {
      close(server_socket_fd);
      connection_loop(&client_info);
      close(client_info.socket_fd);
      debug("Close connection from %s:%d", client_info.ip, client_info.port);
      if(Callback_disconnect != NULL)
        (*Callback_disconnect)(&client_info, Callback_extra_data);
      exit(EXIT_SUCCESS);
    }
    close(client_info.socket_fd);
  }
}
