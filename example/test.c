#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wssl.h"

#define ERROR(format, ...)                \
do                                        \
{                                         \
  fprintf(stderr, format, ##__VA_ARGS__); \
  exit(EXIT_FAILURE);                     \
}                                         \
while(false)                              \

#define CALL(...)                            \
do                                           \
{                                            \
  wssl_result_t _result_ = (__VA_ARGS__);    \
  if(wssl_result_is_not_ok(_result_))        \
    ERROR                                    \
    (                                        \
      "\"%s\" on \"%s\" with %d:\"%s\"\n",   \
      wssl_result_get_code_string(_result_), \
      _result_.where,                        \
      _result_.last_errno,                   \
      strerror(_result_.last_errno)          \
    );                                       \
}                                            \
while(false)                                 \

int main(void)
{
  WSSL_DECLARE(wssl);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5000));
  CALL(wssl_server_add(&wssl, "0.0.0.0", 6000));
  CALL(wssl_loop(&wssl));
  CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
