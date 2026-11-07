#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"
#include "servers.h"

wssl_callback_result_e for_client(wssl_client_t* client, void* local_extra_data)
{
  wssl_client_send_text(client, (char*)local_extra_data);
  return WSSL_CALLBACK_RESULT_OK;
}

#define OUTPUT_SIZE 1024

wssl_callback_result_e on_start_receiving_frames(wssl_client_t* client)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " = JOIN", client->header.uri.data, client->id.suffix);
  wssl_for_each_client_in_frame_processing_call(wssl_client_get_wssl(client), &for_client, (void*)output);
  return WSSL_CALLBACK_RESULT_OK;
}

wssl_callback_result_e on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " - %s", client->header.uri.data, client->id.suffix, data);
  wssl_for_each_client_in_frame_processing_call(wssl_client_get_wssl(client), &for_client, (void*)output);
  return WSSL_CALLBACK_RESULT_OK;
}

void on_disconnect(wssl_client_t* client, wssl_client_disconnect_reason_e disconnect_reason)
{
  if(wssl_client_disconnect_reason_is_not_error(disconnect_reason))
  {
    char output[OUTPUT_SIZE];
    snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " = LEAVE", client->header.uri.data, client->id.suffix);
    wssl_for_each_client_in_frame_processing_call(wssl_client_get_wssl(client), &for_client, (void*)output);
  }
}

int main(int arguments_count, char* arguments_value[])
{
  WSSL_DECLARE(wssl);

  wssl_set_on_start_receiving_frames_callback(&wssl, &on_start_receiving_frames);
  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);
  wssl_set_on_disconnect_callback(&wssl, &on_disconnect);

  switch(arguments_count)
  {
    case 1:
    {
      wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
      WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 5003));
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
