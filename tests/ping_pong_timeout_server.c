#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

static wssl_callback_result_e on_send_ping_frame(wssl_client_t* client)
{
  (void)client;
  return WSSL_CALLBACK_RESULT_OK;
}

static wssl_callback_result_e on_timeout(wssl_client_t* client, wssl_client_state_e state)
{
  (void)client;
  (void)state;
  return WSSL_CALLBACK_RESULT_DISCONNECT;
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_sleep_in_mseconds(&wssl, 50);
  wssl_set_timeout_in_mseconds(&wssl, 5000);
  wssl_set_on_timeout_callback(&wssl, &on_timeout);
  wssl_set_ping_interval_in_mseconds(&wssl, 200);
  wssl_set_on_send_ping_frame_callback(&wssl, &on_send_ping_frame);
  wssl_set_pong_timeout_in_mseconds(&wssl, 2000);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  wssl_server_configuration_set_port(&server_configuration, 6011);
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
