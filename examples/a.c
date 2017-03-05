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

void on_connect(wssl_connection_info_t* connection_info)
{
  printf("Connect %s:%d -> %s:%d\n", connection_info->client->ip, connection_info->client->port, connection_info->server->ip, connection_info->server->port);
}

void on_disconnect(wssl_connection_info_t* connection_info)
{
  printf("Disconnect %s:%d -> %s:%d\n", connection_info->client->ip, connection_info->client->port, connection_info->server->ip, connection_info->server->port);
}

bool on_tick(wssl_t* wssl)
{
  printf("Tick\n");
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_connect_callback_function(&wssl, &on_connect);
  wssl_set_disconnect_callback_function(&wssl, &on_disconnect);
  wssl_set_tick_callback_function(&wssl, &on_tick);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5000));
  CALL(wssl_server_add(&wssl, "0.0.0.0", 6000));
  CALL(wssl_loop(&wssl));
  CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
