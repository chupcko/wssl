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

bool Work = true;

void on_connect(wssl_client_t* client)
{
  printf("Connect %" PRIu16 ":%" PRIu32 " %s:%d -> %s:%d\n", client->id.prefix, client->id.suffix, client->ip, client->port, client->server->ip, client->server->port);
}

void on_disconnect(wssl_client_t* client)
{
  printf("Disconnect %" PRIu16 ":%" PRIu32 " %s:%d -> %s:%d\n", client->id.prefix, client->id.suffix, client->ip, client->port, client->server->ip, client->server->port);
}

bool on_header(wssl_client_t* client)
{
  printf("Header %" PRIu16 ":%" PRIu32 " %s:%d -> %s:%d\n", client->id.prefix, client->id.suffix, client->ip, client->port, client->server->ip, client->server->port);
  return true;
}

bool on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  printf
  (
    (
      "Receive text frame %" PRIu16 ":%" PRIu32 " %s:%d -> %s:%d\n"
      "\"%s\"\n"
    ),
    client->id.prefix, client->id.suffix, client->ip, client->port, client->server->ip, client->server->port,
    data
  );
  return true;
}

bool on_tick(wssl_t* wssl)
{
  printf("Tick\n");
/*  wssl_dump(wssl, stdout, 0);*/
  printf("\n");
  return Work;
}

void on_end(int signal)
{
  Work = false;
}

int main(void)
{
  WSSL_DECLARE(wssl);

  if(signal(SIGTERM, &on_end) == SIG_ERR)
    ERROR("Cannot signal with %d:\"%s\"", errno, strerror(errno));
  if(signal(SIGINT, &on_end) == SIG_ERR)
    ERROR("Cannot signal with %d:\"%s\"", errno, strerror(errno));

  wssl_set_connect_callback(&wssl, &on_connect);
  wssl_set_disconnect_callback(&wssl, &on_disconnect);
  wssl_set_header_callback(&wssl, &on_header);
  wssl_set_receive_text_frame_callback(&wssl, &on_receive_text_frame);
  wssl_set_tick_callback(&wssl, &on_tick);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5000));
  CALL(wssl_server_add(&wssl, "0.0.0.0", 6000));

  CALL(wssl_loop(&wssl));

  CALL(wssl_clean(&wssl));
  wssl_dump(&wssl, stdout, 0);
  printf("\n");

  return EXIT_SUCCESS;
}
