#include "common.h"

bool Work = true;

void on_connect(wssl_client_t* client)
{
  printf
  (
    "Connect %" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX " %s:%d -> %s:%d\n",
    client->id.prefix,
    client->id.suffix,
    client->ip,
    client->port,
    client->server->ip,
    client->server->port
  );
}

void on_disconnect(wssl_client_t* client, wssl_client_disconnect_reason_e disconnect_reason)
{
  printf
  (
    "Disconnect (%s) %" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX " %s:%d -> %s:%d\n",
    wssl_client_get_disconnect_reason_string(disconnect_reason),
    client->id.prefix,
    client->id.suffix,
    client->ip,
    client->port,
    client->server->ip,
    client->server->port
  );
}

bool on_header(wssl_client_t* client)
{
  printf
  (
    "Header %" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX " %s:%d -> %s:%d\n",
    client->id.prefix,
    client->id.suffix,
    client->ip,
    client->port,
    client->server->ip,
    client->server->port
  );
  return true;
}

void on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  printf
  (
    "Receive text frame %" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX " %s:%d -> %s:%d\n",
    client->id.prefix,
    client->id.suffix,
    client->ip,
    client->port,
    client->server->ip,
    client->server->port
  );
}

void on_receive_binary_frame(wssl_client_t* client, wssl_octet_t* data, wssl_size_t data_size)
{
  printf
  (
    "Receive binary frame %" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX " %s:%d -> %s:%d\n",
    client->id.prefix,
    client->id.suffix,
    client->ip, client->port,
    client->server->ip,
    client->server->port
  );
}

void on_receive_close_frame(wssl_client_t* client)
{
  printf
  (
    "Receive close frame %" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX " %s:%d -> %s:%d\n",
    client->id.prefix,
    client->id.suffix,
    client->ip, client->port,
    client->server->ip,
    client->server->port
  );
}

void on_receive_ping_frame(wssl_client_t* client, wssl_octet_t* data, wssl_size_t data_size)
{
  printf
  (
    "Receive binary frame %" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX " %s:%d -> %s:%d\n",
    client->id.prefix,
    client->id.suffix,
    client->ip, client->port,
    client->server->ip,
    client->server->port
  );
}

void on_receive_pong_frame(wssl_client_t* client, wssl_octet_t* data, wssl_size_t data_size)
{
  printf
  (
    "Receive binary frame %" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX " %s:%d -> %s:%d\n",
    client->id.prefix,
    client->id.suffix,
    client->ip, client->port,
    client->server->ip,
    client->server->port
  );
}

bool on_tick(wssl_t* wssl)
{
  printf("Tick\n");
  wssl_dump(wssl, stdout, 0);
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
  wssl_set_receive_binary_frame_callback(&wssl, &on_receive_binary_frame);
  wssl_set_receive_close_frame_callback(&wssl, &on_receive_close_frame);
  wssl_set_receive_ping_frame_callback(&wssl, &on_receive_ping_frame);
  wssl_set_receive_pong_frame_callback(&wssl, &on_receive_pong_frame);
  wssl_set_tick_callback(&wssl, &on_tick);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5000));
  CALL(wssl_server_add(&wssl, "0.0.0.0", 6000));
  CALL(wssl_loop(&wssl));
  CALL(wssl_clean(&wssl));

  wssl_dump(&wssl, stdout, 0);
  printf("\n");

  return EXIT_SUCCESS;
}
