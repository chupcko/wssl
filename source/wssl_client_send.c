#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_send
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_octet_t*  data,
  _WSSL_IN_     const wssl_size_t    data_size
)
{
  if(wssl_buffer_is_created(&client->output_buffer))
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
          WSSL_CALL(wssl_buffer_create(&client->input_buffer, BUFFER_SIZE_IN_OCTETS));
          WSSL_CALL(wssl_buffer_append(&client->output_buffer, data, data_size));
          WSSL_CALL(wssl_client_epoll_event_add_out(client));
          break;
        case ECONNRESET:
        case EPIPE:
          WSSL_CALL(wssl_client_delete(client));
          break;
        default:
          return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "send", errno);
          break;
      }
    else if(send_size == 0)
      WSSL_CALL(wssl_client_delete(client));
    else if((wssl_size_t)send_size < data_size)
    {
      WSSL_CALL(wssl_buffer_create(&client->input_buffer, BUFFER_SIZE_IN_OCTETS));
      WSSL_CALL(wssl_buffer_append(&client->output_buffer, &data[send_size], data_size-send_size));
      WSSL_CALL(wssl_client_epoll_event_add_out(client));
    }
  }
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
