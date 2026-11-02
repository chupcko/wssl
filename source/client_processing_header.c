#include "main.h"

static inline
bool wssl_key_is_base64
(
  _WSSL_IN_ const char*      data,
  _WSSL_IN_ const wssl_size_t data_length
)
{
  wssl_size_t i;
  for(i = 0; i < data_length; i++)
  {
    char c = data[i];
    bool alphabet =
      (c >= 'A' && c <= 'Z') ||
      (c >= 'a' && c <= 'z') ||
      (c >= '0' && c <= '9') ||
      c == '+' || c == '/';
    if(i < data_length-2)
    {
      if(!alphabet)
        return false;
    }
    else if(c != '=')
      return false;
  }
  return true;
}

static inline
bool wssl_client_processing_header_check
(
  _WSSL_MODIFY_ wssl_client_t*  client,
  _WSSL_OUT_    wssl_string_t** sec_websocket_key
)
{
  if
  (
    wssl_string_is_not_allocated(&client->header.method) ||
    strcmp(client->header.method.data, "GET") != 0 ||
    wssl_header_get_field_value(&client->header, "Host") == WSSL_NULL
  )
    return false;

  wssl_string_t* connection = wssl_header_get_field_value(&client->header, "Connection");
  if
  (
    connection == WSSL_NULL ||
    wssl_string_is_not_allocated(connection) ||
    !wssl_str_case_contains(connection->data, "upgrade")
  )
    return false;

  wssl_string_t* upgrade = wssl_header_get_field_value(&client->header, "Upgrade");
  if
  (
    upgrade == WSSL_NULL ||
    wssl_string_is_not_allocated(upgrade) ||
    !wssl_str_case_contains(upgrade->data, "websocket")
  )
    return false;

  wssl_string_t* version = wssl_header_get_field_value(&client->header, "Sec-WebSocket-Version");
  if
  (
    version == WSSL_NULL ||
    wssl_string_is_not_allocated(version) ||
    strcmp(version->data, "13") != 0
  )
    return false;

  *sec_websocket_key = wssl_header_get_field_value(&client->header, "Sec-WebSocket-Key");
  if
  (
    *sec_websocket_key == WSSL_NULL ||
    wssl_string_is_not_allocated(*sec_websocket_key) ||
    (*sec_websocket_key)->data_length != 24 ||
    !wssl_key_is_base64((*sec_websocket_key)->data, (*sec_websocket_key)->data_length)
  )
    return false;

  return true;
}

_FUNCTION_
wssl_result_t wssl_client_processing_header
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  bool pass = true;
  if(client->wssl->on_receive_header_callback != WSSL_CALLBACK_NONE)
  {
    pass = (*client->wssl->on_receive_header_callback)(client);
    PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
  }

  wssl_string_t* sec_websocket_key;
  if(pass)
    pass = wssl_client_processing_header_check(client, &sec_websocket_key);

  wssl_chunk_t* chunk;
  TRY_CALL_ELSE_MARK_FOR_DISCONNECTING(client, WSSL_CLIENT_DISCONNECT_REASON_NO_MEMORY, wssl_chunk_add(client, client->wssl->buffer_size_in_octets, &chunk));
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  if(pass)
  {
    char handshake_result[HANDSHAKE_RESULT_SIZE];
    wssl_size_t handshake_result_length;
    TRY_CALL_ELSE_MARK_FOR_DISCONNECTING
    (
      client,
      WSSL_CLIENT_DISCONNECT_REASON_NO_MEMORY,
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

  TRY_CALL(wssl_client_do_send(client));
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  if(!pass)
    MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_BAD_HEADER);

  client->state = WSSL_CLIENT_STATE_WAIT_FRAME;
  wssl_client_chain_delete_link(&client->wssl_chain_link);
  wssl_client_chain_add_link_backward(&client->wssl->clients_in_frame_processing, &client->wssl_chain_link);
  if(client->wssl->on_start_receiving_frames_callback != WSSL_CALLBACK_NONE)
  {
    (*client->wssl->on_start_receiving_frames_callback)(client);
    PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
  }

  PASS;
}
