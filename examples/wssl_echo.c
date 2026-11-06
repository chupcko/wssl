#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

wssl_callback_result_e on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  wssl_client_send_string(client, &WSSL_MAKE_STRING(data, data_size));
  return WSSL_CALLBACK_RESULT_OK;
}

int main(int arguments_count, char* arguments_value[])
{
  if(arguments_count != 1 && arguments_count != 3)
  {
    fprintf(stderr, "usage: %s [certificate_file private_key_file]\n", arguments_value[0]);
    return EXIT_FAILURE;
  }

  WSSL_DECLARE(wssl);

  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  if(arguments_count == 3)
  {
    WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 5011));
    WSSL_TRY_CALL(wssl_server_configuration_enable_ssl(&server_configuration, arguments_value[1], arguments_value[2]));
  }
  else
  {
    WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 5001));
  }
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
