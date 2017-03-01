#include "main.h"

void callback_connect_function(client_info_t* client_info, void* extra_data)
{
  extra_data_t* ed = (extra_data_t*)extra_data;

  debug("%s:%d in connect", client_info->ip, client_info->port);
  ed->periodic_last_time = time(NULL);
}

void callback_disconnect_function(client_info_t* client_info, void* extra_data)
{
  debug("%s:%d in disconnect", client_info->ip, client_info->port);
}

bool callback_periodic_function(client_info_t* client_info, void* extra_data)
{
  extra_data_t* ed = (extra_data_t*)extra_data;
  time_t now = time(NULL);

  if
  (
    client_info->in_header != true &&
    now > ed->periodic_last_time+EXTRA_DATA_PAUSE
  )
  {
    debug("%s:%d in periodic", client_info->ip, client_info->port);
    frame_send_text(client_info, "periodic");
    ed->periodic_last_time = now;
  }
  return true;
}

bool callback_recv_function(client_info_t* client_info, void* extra_data, uint8_t* message, int message_size)
{
  debug("%s:%d in recv message=\"%s\"", client_info->ip, client_info->port, message);
  frame_send_text(client_info, message);
  return true;
}
