#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do_recv
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  if(wssl_buffer_is_not_allocated(&client->input_buffer))
    TRY_CALL(wssl_buffer_allocate(&client->input_buffer, client->wssl->buffer_size_in_octets));

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
        PASS;
        break;
      case ECONNRESET:
      case EPIPE:
        MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
        break;
      default:
        return MAKE_RESULT_ERRNO("recv", errno);
        break;
    }
  if(recv_size == 0)
    MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);

  client->input_buffer.end += (wssl_size_t)recv_size;

  wssl_size_t processed;
  while(wssl_buffer_is_not_empty(&client->input_buffer))
  {
    TRY_CALL
    (
      wssl_client_processing_recv
      (
        client,
        &client->input_buffer.data[client->input_buffer.begin],
        client->input_buffer.end-client->input_buffer.begin,
        &processed
      )
    );
    PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
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
      MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_INPUT_BUFFER_IS_FULL);
  }

  PASS;
}
