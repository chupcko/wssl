#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wssl.h"

static wssl_callback_result_e on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  if(data_size == 4 && memcmp(data, "kick", 4) == 0)
    return WSSL_CALLBACK_RESULT_DISCONNECT;
  if(data_size == 4 && memcmp(data, "fail", 4) == 0)
    return WSSL_CALLBACK_RESULT_ERROR;
  wssl_client_send_string(client, &WSSL_MAKE_STRING(data, data_size));
  return WSSL_CALLBACK_RESULT_OK;
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 5004));
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
