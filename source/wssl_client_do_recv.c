#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do_recv
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  if(wssl_buffer_is_not_created(&client->input_buffer))
    WSSL_CALL(wssl_buffer_create(&client->input_buffer, BUFFER_SIZE_IN_OCTETS));

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
        WSSL_CALL(wssl_client_delete(client));
        break;
      default:
        return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "recv", errno);
        break;
    }
  else if(recv_size == 0)
    WSSL_CALL(wssl_client_delete(client));
  else
  {
    wssl_size_t processed = 0;
    wssl_ssize_t local_processed;

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
      if(local_processed < 0)
      {
        WSSL_CALL(wssl_client_delete(client));
        return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
      }
      else if(local_processed == 0)
        break;
      else
        processed += (wssl_size_t)local_processed;
    }
    if(processed == client->input_buffer.used)
      wssl_buffer_clean(&client->input_buffer);
    else if(processed > 0)
      wssl_buffer_shift(&client->input_buffer, processed);
    else if(client->input_buffer.used == client->input_buffer.size)
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "input_buffer", 0);
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
