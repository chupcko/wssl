#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do_send
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_OUT_    bool*          client_deleted
)
{
  *client_deleted = false;

  if(wssl_buffer_is_created(&client->output_buffer))
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
          WSSL_CALL(wssl_client_delete(client));
          *client_deleted = true;
          break;
        default:
          return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "send", errno);
          break;
      }
    else if(send_size == 0)
    {
      WSSL_CALL(wssl_client_delete(client));
      *client_deleted = true;
    }
    else if((wssl_size_t)send_size == client->output_buffer.used)
    {
      wssl_buffer_clean(&client->output_buffer);
      WSSL_CALL(wssl_client_epoll_event_delete_out(client));
    }
    else
      wssl_buffer_shift(&client->output_buffer, (wssl_size_t)send_size);
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
