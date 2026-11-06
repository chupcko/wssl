#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "wssl.h"

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

int main(int arguments_count, char* arguments_value[])
{
  if(arguments_count != 1 && arguments_count != 3)
  {
    fprintf(stderr, "usage: %s [certificate_file private_key_file]\n", arguments_value[0]);
    return EXIT_FAILURE;
  }

  WSSL_DECLARE(wssl);

  wssl_set_sleep_in_mseconds(&wssl, 500);
  wssl_set_on_tick_callback(&wssl, &on_tick);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  if(arguments_count == 3)
  {
    WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 5012));
    WSSL_TRY_CALL(wssl_server_configuration_enable_ssl(&server_configuration, arguments_value[1], arguments_value[2]));
  }
  else
  {
    WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 5002));
  }
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
