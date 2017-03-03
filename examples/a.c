#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wssl.h"

#define CALL(...)                          \
do                                         \
{                                          \
  wssl_result_t _result_;                  \
                                           \
  _result_ = (__VA_ARGS__);                \
  if(_result_.code != WSSL_RESULT_CODE_OK) \
  {                                        \
    fprintf                                \
    (                                      \
      stderr,                              \
      "\"%s\" %d \"%s\"\n",                \
      wssl_get_result_string(_result_),    \
      _result_.last_errno,                 \
      strerror(_result_.last_errno)        \
    );                                     \
    exit(EXIT_FAILURE);                    \
  }                                        \
}                                          \
while(0)                                   \

int main(void)
{
  WSSL_DECLARE(wssl);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5000));
  CALL(wssl_server_add(&wssl, "0.0.0.0", 6000));

  CALL(wssl_loop(&wssl));

  wssl_clean(&wssl);
  return EXIT_SUCCESS;
}
