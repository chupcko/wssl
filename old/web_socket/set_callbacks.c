#include "main.h"

void set_callback_extra_data(void* extra_data)
{
  Callback_extra_data = extra_data;
}

void set_callback_connect(callback_connect_t* function)
{
  Callback_connect = function;
}

void set_callback_disconnect(callback_disconnect_t* function)
{
  Callback_disconnect = function;
}

void set_callback_periodic(callback_periodic_t* function)
{
  Callback_periodic = function;
}

void set_callback_poll(callback_poll_t* function, int fd)
{
  Callback_poll = function;
  Callback_poll_fd = fd;
}

void set_callback_recv(callback_recv_t* function)
{
  Callback_recv = function;
}
