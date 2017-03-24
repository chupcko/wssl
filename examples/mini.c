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

void on_client(wssl_client_t* client)
{
  #define DATA_SIZE 64
  char data[DATA_SIZE];

  snprintf(data, DATA_SIZE, "t=%ld", (long)time(NULL));
  wssl_client_send_text(client, data);
  #undef DATA_SIZE
}

bool on_tick(wssl_t* wssl)
{
  wssl_for_each_client_call(wssl, &on_client);
  return true;
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_sleep_in_mseconds(&wssl, 5000);
  wssl_set_receive_text_frame_callback(&wssl, &on_receive_text_frame);
  wssl_set_tick_callback(&wssl, &on_tick);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5000));
  CALL(wssl_loop(&wssl));
  CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
