#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do_recv
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  if(wssl_client_is_disconnected(client))
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);

  if(wssl_buffer_is_not_allocated(&client->input_buffer))
    WSSL_CALL(wssl_buffer_allocate(&client->input_buffer, client->wssl->buffer_size_in_octets));

  wssl_ssize_t recv_size = (wssl_ssize_t)recv
  (
    client->socket_descriptor,
    (void*)&client->input_buffer.data[client->input_buffer.used],
    (size_t)(client->input_buffer.size-client->input_buffer.used),
    0
  );
  if(recv_size < 0)
    switch(errno)
    {
      case EAGAIN:
        break;
      case ECONNRESET:
      case EPIPE:
        wssl_client_disconnect(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
        break;
      default:
        return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "recv", errno);
        break;
    }
  else if(recv_size == 0)
    wssl_client_disconnect(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);
  else
  {
    wssl_size_t processed = 0;
    wssl_size_t local_processed;

    client->input_buffer.used += (wssl_size_t)recv_size;
    while(processed < client->input_buffer.used)
    {
      WSSL_CALL
      (
        wssl_client_processing_recv
        (
          client,
          &client->input_buffer.data[processed],
          client->input_buffer.used-processed,
          &local_processed
        )
      );
      if(wssl_client_is_disconnected(client))
        return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
      if(local_processed == 0)
        break;
      processed += local_processed;
    }
    if(processed == client->input_buffer.used)
      wssl_buffer_free(&client->input_buffer);
    else if(processed > 0)
      wssl_buffer_shift(&client->input_buffer, processed);
    else if(wssl_buffer_is_full(&client->input_buffer))
      wssl_client_disconnect(client, WSSL_CLIENT_DISCONNECT_REASON_FULL_RECV_BUFFER);
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
