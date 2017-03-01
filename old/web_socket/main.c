#define _MAIN_C_

#include "main.h"

int main(void)
{
  int server_socket_fd;

  srand(time(NULL));
  if(signal(SIGCHLD, &signal_handler_child_end) == SIG_ERR)
    error("Cannot signal(%d %s)", errno, strerror(errno));

  set_callback_extra_data(&Extra_data);
  set_callback_connect(&callback_connect_function);
  set_callback_disconnect(&callback_disconnect_function);
  set_callback_periodic(&callback_periodic_function);
  set_callback_recv(&callback_recv_function);

  server_socket_fd = server_init(WEBSOCKET_SERVER_PORT);
  server_loop(server_socket_fd);

  close(server_socket_fd);

  return EXIT_SUCCESS;
}
