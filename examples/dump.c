#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

#define ERROR(format, ...)                \
do                                        \
{                                         \
  fprintf(stderr, format, ##__VA_ARGS__); \
  fprintf(stderr, "\n");                  \
  exit(EXIT_FAILURE);                     \
}                                         \
while(false)                              \

bool Work = true;

wssl_callback_result_e on_connect(wssl_client_t* client)
{
  printf("Connect ");
  wssl_client_print(client, stdout);
  printf("\n");
  return WSSL_CALLBACK_RESULT_OK;
}

void on_disconnect(wssl_client_t* client, wssl_client_disconnect_reason_e disconnect_reason)
{
  printf("Disconnect (%s) ", wssl_client_get_disconnect_reason_name(disconnect_reason));
  wssl_client_print(client, stdout);
  printf("\n");
}

wssl_callback_result_e on_receive_header(wssl_client_t* client)
{
  printf("Receive header ");
  wssl_client_print(client, stdout);
  printf("\n");
  return WSSL_CALLBACK_RESULT_OK;
}

wssl_callback_result_e on_start_receiving_frames(wssl_client_t* client)
{
  printf("Start receiving frames ");
  wssl_client_print(client, stdout);
  printf("\n");
  return WSSL_CALLBACK_RESULT_OK;
}

wssl_callback_result_e on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  printf("Receive text frame ");
  wssl_client_print(client, stdout);
  printf("\n");
  return WSSL_CALLBACK_RESULT_OK;
}

wssl_callback_result_e on_receive_binary_frame(wssl_client_t* client, wssl_octet_t* data, wssl_size_t data_size)
{
  printf("Receive binary frame ");
  wssl_client_print(client, stdout);
  printf("\n");
  return WSSL_CALLBACK_RESULT_OK;
}

wssl_callback_result_e on_receive_close_frame(wssl_client_t* client)
{
  printf("Receive close frame ");
  wssl_client_print(client, stdout);
  printf("\n");
  return WSSL_CALLBACK_RESULT_OK;
}

wssl_callback_result_e on_receive_ping_frame(wssl_client_t* client, wssl_octet_t* data, wssl_size_t data_size)
{
  printf("Receive ping frame ");
  wssl_client_print(client, stdout);
  printf("\n");
  return WSSL_CALLBACK_RESULT_OK;
}

wssl_callback_result_e on_receive_pong_frame(wssl_client_t* client, wssl_octet_t* data, wssl_size_t data_size)
{
  printf("Receive pong frame ");
  wssl_client_print(client, stdout);
  printf("\n");
}

bool on_tick(wssl_t* wssl)
{
  printf("Tick\n");

#ifdef WSSL_DEBUG
  wssl_dump(wssl, stdout, 0);
  printf("\n");
#endif

  return Work;
}

void on_end(int signal)
{
  Work = false;
}

int main(int arguments_count, char* arguments_value[])
{
  if(arguments_count != 1 && arguments_count != 3)
  {
    fprintf(stderr, "usage: %s [certificate_file private_key_file]\n", arguments_value[0]);
    return EXIT_FAILURE;
  }

  WSSL_DECLARE(wssl);

  if(signal(SIGTERM, &on_end) == SIG_ERR)
  {
    ERROR("Cannot signal with %d:\"%s\"", errno, strerror(errno));
  }
  if(signal(SIGINT, &on_end) == SIG_ERR)
  {
    ERROR("Cannot signal with %d:\"%s\"", errno, strerror(errno));
  }

  wssl_set_on_connect_callback(&wssl, &on_connect);
  wssl_set_on_disconnect_callback(&wssl, &on_disconnect);
  wssl_set_on_receive_header_callback(&wssl, &on_receive_header);
  wssl_set_on_start_receiving_frames_callback(&wssl, &on_start_receiving_frames);
  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);
  wssl_set_on_receive_binary_frame_callback(&wssl, &on_receive_binary_frame);
  wssl_set_on_receive_close_frame_callback(&wssl, &on_receive_close_frame);
  wssl_set_on_receive_ping_frame_callback(&wssl, &on_receive_ping_frame);
  wssl_set_on_receive_pong_frame_callback(&wssl, &on_receive_pong_frame);
  wssl_set_on_tick_callback(&wssl, &on_tick);

  wssl_server_configuration_t server_configuration = WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK;
  if(arguments_count == 3)
  {
    wssl_server_configuration_set_port(&server_configuration, 6011);
    wssl_server_configuration_enable_ssl(&server_configuration, arguments_value[1], arguments_value[2]);
  }
  else
  {
    wssl_server_configuration_set_port(&server_configuration, 6001);
  }
  WSSL_TRY_CALL(wssl_server_add(&wssl, &server_configuration));
  WSSL_TRY_CALL(wssl_loop_init_signals());
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

#ifdef WSSL_DEBUG
  wssl_dump(&wssl, stdout, 0);
  printf("\n");
#endif

  return EXIT_SUCCESS;
}
