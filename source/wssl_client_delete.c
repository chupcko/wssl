#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_delete
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  if(client->wssl->disconnect_callback_function != WSSL_CALLBACK_FUNCTION_NONE)
    (*client->wssl->disconnect_callback_function)(client);

  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_DEL, client->socket_descriptor, NULL) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);

  if(close(client->socket_descriptor) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "close", errno);
  client->socket_descriptor = WSSL_NO_DESCRIPTOR;

  if(wssl_buffer_is_created(&client->input_buffer))
    wssl_buffer_clean(&client->input_buffer);
  if(wssl_buffer_is_created(&client->output_buffer))
    wssl_buffer_clean(&client->output_buffer);

  wssl_header_delete(&client->header);

  wssl_chain_delete_link(&client->chain_link);

  free((void*)client);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
