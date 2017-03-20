#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_send
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_octet_t*  data,
  _WSSL_IN_     const wssl_size_t    data_size
)
{
  if(wssl_client_is_to_delete(client))
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);

  if(wssl_buffer_is_allocated(&client->output_buffer))
  {
    WSSL_CALL(wssl_buffer_append(&client->output_buffer, data, data_size));
    WSSL_CALL(wssl_client_do_send(client));
  }
  else
  {
    wssl_ssize_t send_size = (wssl_ssize_t)send
    (
      client->socket_descriptor,
      (void*)data,
      (size_t)data_size,
      0
    );
    if(send_size < 0)
      switch(errno)
      {
        case EAGAIN:
          WSSL_CALL(wssl_buffer_allocate(&client->input_buffer, client->wssl->buffer_size_in_octets));
          WSSL_CALL(wssl_buffer_append(&client->output_buffer, data, data_size));
          WSSL_CALL(wssl_client_epoll_event_add_out(client));
          break;
        case ECONNRESET:
        case EPIPE:
          wssl_client_to_delete(client, WSSL_CLIENT_DELETE_REASON_DISCONNECTED);
          break;
        default:
          return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "send", errno);
          break;
      }
    else if(send_size == 0)
      wssl_client_to_delete(client, WSSL_CLIENT_DELETE_REASON_CLOSED);
    else if((wssl_size_t)send_size < data_size)
    {
      WSSL_CALL(wssl_buffer_allocate(&client->input_buffer, client->wssl->buffer_size_in_octets));
      WSSL_CALL(wssl_buffer_append(&client->output_buffer, &data[send_size], data_size-send_size));
      WSSL_CALL(wssl_client_epoll_event_add_out(client));
    }
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
