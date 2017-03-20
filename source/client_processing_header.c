#include "main.h"

static inline bool wssl_client_processing_header_check
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_OUT_    char**         sec_websocket_key
)
{
  if
  (
    (
      client->wssl->header_callback != WSSL_CALLBACK_NONE &&
      !(*client->wssl->header_callback)(client)
    ) ||
    client->header.method == WSSL_NULL ||
    strcmp(client->header.method, "GET") != 0 ||
    wssl_header_get_field_value(&client->header, "Host") == WSSL_NULL
  )
    return false;

  char* connection = wssl_header_get_field_value(&client->header, "Connection");
  if
  (
    connection == WSSL_NULL ||
    strstr(connection, "Upgrade") == NULL
  )
    return false;

  char* upgrade = wssl_header_get_field_value(&client->header, "Upgrade");
  if
  (
    upgrade == WSSL_NULL ||
    strcmp(upgrade, "websocket") != 0
  )
    return false;

  *sec_websocket_key = wssl_header_get_field_value(&client->header, "Sec-WebSocket-Key");
  return *sec_websocket_key != WSSL_NULL;
}

_FUNCTION_
wssl_result_t wssl_client_processing_header
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  wssl_buffer_t buffer;
  WSSL_CALL(wssl_buffer_allocate(&buffer, client->wssl->buffer_size_in_octets));

  bool must_client_delete = false;
  char* sec_websocket_key;
  if(wssl_client_processing_header_check(client, &sec_websocket_key))
  {
    char handshake_result[HANDSHAKE_RESULT_SIZE];
    wssl_size_t handshake_result_length;
    WSSL_CALL
    (
      wssl_handshake
      (
        sec_websocket_key,
        strlen(sec_websocket_key),/*#*/
        handshake_result,
        HANDSHAKE_RESULT_SIZE,
        &handshake_result_length
      )
    );
    wssl_buffer_printf
    (
      &buffer,
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
  {
    wssl_buffer_printf
    (
      &buffer,
      (
        "HTTP/1.1 400 Bad Request\r\n"
        "Server: %s\r\n"
        "\r\n"
        "Bad Request\n"
      ),
      client->wssl->server_name
    );
    must_client_delete = true;
  }

  wssl_result_t result = wssl_client_send(client, buffer.data, buffer.used);
  wssl_buffer_free(&buffer);
  if(wssl_result_is_not_ok(result))
    return result;
  if
  (
    wssl_client_is_not_to_delete(client) &&
    must_client_delete
  )
    wssl_client_to_delete(client, WSSL_CLIENT_DELETE_REASON_BAD_HANDSHAKE);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
