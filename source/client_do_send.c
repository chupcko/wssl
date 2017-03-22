#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do_send
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  if(wssl_client_is_for_disconnecting(client))
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);

  if(wssl_buffer_is_allocated(&client->output_buffer))
  {
    wssl_ssize_t send_size = (wssl_ssize_t)send
    (
      client->socket_descriptor,
      (void*)client->output_buffer.data,
      (size_t)client->output_buffer.used,
      0
    );
    if(send_size < 0)
      switch(errno)
      {
        case EAGAIN:
          break;
        case ECONNRESET:
        case EPIPE:
          wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
          break;
        default:
          return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "send", errno);
          break;
      }
    else if(send_size == 0)
      wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);
    else if((wssl_size_t)send_size == client->output_buffer.used)
    {
      wssl_buffer_free(&client->output_buffer);
      WSSL_CALL(wssl_client_epoll_event_delete_out(client));
    }
    else
      wssl_buffer_shift(&client->output_buffer, (wssl_size_t)send_size);
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
