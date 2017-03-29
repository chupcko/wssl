#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do_recv
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  if(wssl_client_is_for_disconnecting(client))
    return WSSL_MAKE_RESULT_OK;

  if(wssl_buffer_is_not_allocated(&client->input_buffer))
    WSSL_CALL(wssl_buffer_allocate(&client->input_buffer, client->wssl->buffer_size_in_octets));

  wssl_ssize_t recv_size = (wssl_ssize_t)recv
  (
    client->socket_descriptor,
    (void*)&client->input_buffer.data[client->input_buffer.end],
    (size_t)(client->input_buffer.size-client->input_buffer.end),
    0
  );
  if(recv_size < 0)
    switch(errno)
    {
      case EAGAIN:
        break;
      case ECONNRESET:
      case EPIPE:
        wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
        break;
      default:
        return WSSL_MAKE_RESULT_ERRNO("recv", errno);
        break;
    }
  else if(recv_size == 0)
    wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);
  else
  {
    wssl_size_t processed;

    client->input_buffer.end += (wssl_size_t)recv_size;
    while(wssl_buffer_is_not_empty(&client->input_buffer))
    {
      WSSL_CALL
      (
        wssl_client_processing_recv
        (
          client,
          &client->input_buffer.data[client->input_buffer.begin],
          client->input_buffer.end-client->input_buffer.begin,
          &processed
        )
      );
      if(wssl_client_is_for_disconnecting(client))
        return WSSL_MAKE_RESULT_OK;
      if(processed == 0)
        break;
      client->input_buffer.begin += processed;
    }

    if(wssl_buffer_is_empty(&client->input_buffer))
      wssl_buffer_free(&client->input_buffer);
    else
    {
      wssl_buffer_shift_left(&client->input_buffer);
      if(wssl_buffer_is_full(&client->input_buffer))
        wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_FULL_RECV_BUFFER);
    }
  }

  return WSSL_MAKE_RESULT_OK;
}
