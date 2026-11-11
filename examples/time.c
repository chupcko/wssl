#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "wssl.h"
#include "servers.h"

#define OUTPUT_SIZE 64

wssl_callback_result_e for_client(wssl_client_t* client, void* local_extra_data)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "t=%ld", (long)time(NULL));
  wssl_client_send_text(client, output);
  return WSSL_CALLBACK_RESULT_OK;
}

bool on_tick(wssl_t* wssl)
{
  wssl_for_each_client_in_frame_processing_call(wssl, &for_client, NULL);
  return true;
}

wssl_callback_result_e on_timeout(wssl_client_t* client, wssl_client_state_e state)
{
  return WSSL_CALLBACK_RESULT_DISCONNECT;
}

int main(int arguments_count, char* arguments_value[])
{
  WSSL_DECLARE(wssl);

  wssl_set_sleep_in_mseconds(&wssl, 500);
  wssl_set_on_tick_callback(&wssl, &on_tick);

  wssl_set_timeout_in_mseconds(&wssl, 10000);
  wssl_set_on_timeout_callback(&wssl, &on_timeout);

  switch(arguments_count)
  {
    case 1:
    {
      wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
      WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 5002));
      WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
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
