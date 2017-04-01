#include "main.h"

static inline
bool wssl_client_processing_header_check
(
  _WSSL_MODIFY_ wssl_client_t*  client,
  _WSSL_OUT_    wssl_string_t** sec_websocket_key
)
{
  if
  (
    (
      client->wssl->header_callback != WSSL_CALLBACK_NONE &&
      !(*client->wssl->header_callback)(client)
    ) ||
    wssl_string_is_not_allocated(&client->header.method) ||
    strcmp(client->header.method.data, "GET") != 0 ||
    wssl_header_get_field_value(&client->header, "Host") == WSSL_NULL
  )
    return false;

  wssl_string_t* connection = wssl_header_get_field_value(&client->header, "Connection");
  if
  (
    wssl_string_is_not_allocated(connection) ||
    strstr(connection->data, "Upgrade") == NULL
  )
    return false;

  wssl_string_t* upgrade = wssl_header_get_field_value(&client->header, "Upgrade");
  if
  (
    wssl_string_is_not_allocated(upgrade) ||
    strcmp(upgrade->data, "websocket") != 0
  )
    return false;

  *sec_websocket_key = wssl_header_get_field_value(&client->header, "Sec-WebSocket-Key");
  return wssl_string_is_allocated(*sec_websocket_key);
}

_FUNCTION_
wssl_result_t wssl_client_processing_header
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  wssl_string_t* sec_websocket_key;
  bool pass_header = wssl_client_processing_header_check(client, &sec_websocket_key);

  wssl_chunk_t* chunk;
  WSSL_CALL(wssl_chunk_add(client, client->wssl->buffer_size_in_octets, &chunk));

  if(pass_header)
  {
    char handshake_result[HANDSHAKE_RESULT_SIZE];
    wssl_size_t handshake_result_length;
    WSSL_CALL
    (
      wssl_handshake
      (
        sec_websocket_key->data,
        sec_websocket_key->data_length,
        handshake_result,
        HANDSHAKE_RESULT_SIZE,
        &handshake_result_length
      )
    );
    wssl_buffer_printf
    (
      &chunk->buffer,
      (
        "HTTP/1.1 101 Web Socket Protocol Handshake\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: %s\r\n"
        "Server: %s\r\n"
        "\r\n"
      ),
      handshake_result,
      client->wssl->server_name
    );
  }
  else
    wssl_buffer_printf
    (
      &chunk->buffer,
      (
        "HTTP/1.1 400 Bad Request\r\n"
        "Server: %s\r\n"
        "\r\n"
        "Bad Request\n"
      ),
      client->wssl->server_name
    );

  WSSL_CALL(wssl_client_do_send(client));
  if
  (
    !pass_header &&
    wssl_client_is_not_for_disconnecting(client)
  )
    wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_BAD_HANDSHAKE);

  return WSSL_MAKE_RESULT_OK;
}
