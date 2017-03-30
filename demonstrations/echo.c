#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "wssl.h"

#define CALL(...)                         \
do                                        \
{                                         \
  wssl_result_t _result_ = (__VA_ARGS__); \
  if(wssl_result_is_not_ok(_result_))     \
  {                                       \
    wssl_result_dump(_result_, stderr);   \
    exit(EXIT_FAILURE);                   \
  }                                       \
}                                         \
while(false)                              \

void on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  wssl_client_send_text(client, data);
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_receive_text_frame_callback(&wssl, &on_receive_text_frame);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5001));
  CALL(wssl_loop(&wssl));
  CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
