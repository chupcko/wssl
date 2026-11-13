#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

static wssl_callback_result_e on_send_ping_frame(wssl_client_t* client)
{
  (void)client;
  return WSSL_CALLBACK_RESULT_DISCONNECT;
}

static wssl_callback_result_e on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  (void)data;
  (void)data_size;
  WSSL_TRY_CALL(wssl_client_send_ping(client));
  return WSSL_CALLBACK_RESULT_OK;
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_sleep_in_mseconds(&wssl, 50);
  wssl_set_timeout_in_mseconds(&wssl, 2000);
  wssl_set_on_send_ping_frame_callback(&wssl, &on_send_ping_frame);
  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  wssl_server_configuration_set_port(&server_configuration, 6010);
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
