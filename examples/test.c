#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

int main(int arguments_count, char* arguments_value[])
{
  if(arguments_count != 1 && arguments_count != 3)
  {
    fprintf(stderr, "usage: %s [certificate_file private_key_file]\n", arguments_value[0]);
    return EXIT_FAILURE;
  }

  WSSL_DECLARE(wssl);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  if(arguments_count == 3)
  {
    WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 6011));
    WSSL_TRY_CALL(wssl_server_configuration_enable_ssl(&server_configuration, arguments_value[1], arguments_value[2]));
  }
  else
  {
    WSSL_TRY_CALL(wssl_server_configuration_set_port(&server_configuration, 6001));
  }
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
