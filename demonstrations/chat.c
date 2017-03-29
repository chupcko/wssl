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

#define OUTPUT_SIZE 1024

void on_client(wssl_client_t* client, void* local_extra_data)
{
  wssl_client_send_text(client, (char*)local_extra_data);
}

bool on_header(wssl_client_t* client)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " = JOIN", client->header.uri, client->id.suffix);
  wssl_for_each_client_call(wssl_client_get_wssl(client), &on_client, (void*)output);
  return true;
}

void on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " - %s", client->header.uri, client->id.suffix, data);
  wssl_for_each_client_call(wssl_client_get_wssl(client), &on_client, (void*)output);
}

void on_disconnect(wssl_client_t* client, wssl_client_disconnect_reason_e disconnect_reason)
{
  if(disconnect_reason == WSSL_CLIENT_DISCONNECT_REASON_REQUESTED)
  {
    char output[OUTPUT_SIZE];
    snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " = LEAVE", client->header.uri, client->id.suffix);
    wssl_for_each_client_call(wssl_client_get_wssl(client), &on_client, (void*)output);
  }
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_header_callback(&wssl, &on_header);
  wssl_set_receive_text_frame_callback(&wssl, &on_receive_text_frame);
  wssl_set_disconnect_callback(&wssl, &on_disconnect);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5002));
  CALL(wssl_loop(&wssl));
  CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
