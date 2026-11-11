#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_sleep_in_mseconds(&wssl, 50);
  wssl_set_timeout_in_mseconds(&wssl, 400);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 6006));
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
