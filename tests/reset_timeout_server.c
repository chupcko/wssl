#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

#define RESET_WINDOW_IN_MSECONDS 2000

static wssl_mseconds_t start_in_mseconds;

static wssl_callback_result_e reset_client(wssl_client_t* client, void* local_extra_data)
{
  (void)local_extra_data;
  wssl_client_reset_timeout(client);
  return WSSL_CALLBACK_RESULT_OK;
}

static bool on_tick(struct wssl_t* wssl)
{
  if(wssl_now_in_mseconds()-start_in_mseconds < RESET_WINDOW_IN_MSECONDS)
  {
    wssl_for_each_client_in_frame_processing_call(wssl, &reset_client, NULL);
  }
  return true;
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

  wssl_set_sleep_in_mseconds(&wssl, 100);
  wssl_set_timeout_in_mseconds(&wssl, 400);
  wssl_set_on_tick_callback(&wssl, &on_tick);
  wssl_set_on_timeout_callback(&wssl, &on_timeout);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  wssl_server_configuration_set_port(&server_configuration, 6007);
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  start_in_mseconds = wssl_now_in_mseconds();
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
