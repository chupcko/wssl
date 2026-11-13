#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"
#include "servers.h"

wssl_callback_result_e on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  wssl_client_send_string(client, &WSSL_MAKE_STRING(data, data_size));
  return WSSL_CALLBACK_RESULT_OK;
}

int main(int arguments_count, char* arguments_value[])
{
  WSSL_DECLARE(wssl);

  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);

  switch(arguments_count)
  {
    case 1:
    {
      wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
      wssl_server_configuration_set_port(&server_configuration, 5001);
      wssl_server_add(&wssl, &server_configuration);
      break;
    }
    case 2:
    {
      wssl_server_configuration_t servers[SERVERS_MAX_COUNT];
      int server_count = servers_read_configurations(arguments_value[1], servers);
      if(server_count < 0)
      {
        return EXIT_FAILURE;
      }
      int server_index;
      for(server_index = 0; server_index < server_count; server_index++)
      {
        WSSL_TRY_CALL(wssl_server_add(&wssl, &servers[server_index]));
      }
      break;
    }
    default:
    {
      fprintf(stderr, "usage: %s [configuration_file]\n", arguments_value[0]);
      return EXIT_FAILURE;
      break;
    }
  }

  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
