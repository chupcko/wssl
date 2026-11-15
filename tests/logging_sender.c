#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

int main(int arguments_count, char* arguments_value[])
{
  if(arguments_count != 2)
  {
    fprintf(stderr, "usage: %s <syslog-server>\n", arguments_value[0]);
    return EXIT_FAILURE;
  }

  WSSL_DECLARE(wssl);

  wssl_set_logging_level(&wssl, WSSL_LOGGING_LEVEL_DEBUG);
  wssl_set_syslog_server(&wssl, arguments_value[1]);
  wssl_set_syslog_ident(&wssl, "logtest");
  wssl_logging_start(&wssl);

  wssl_logging(&wssl, WSSL_LOGGING_LEVEL_ERROR, "where1", "hello error");
  wssl_logging(&wssl, WSSL_LOGGING_LEVEL_DEBUG, "where1", "hello debug");

  wssl_set_logging_level(&wssl, WSSL_LOGGING_LEVEL_ERROR);
  wssl_logging(&wssl, WSSL_LOGGING_LEVEL_DEBUG, "where1", "only on stderr");

  wssl_logging_end(&wssl);
  (void)wssl_clean(&wssl);

  return EXIT_SUCCESS;
}
