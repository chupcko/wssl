#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_delete
(
  _WSSL_MODIFY_ wssl_t*        wssl,
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  if(wssl->disconnect_callback_function != WSSL_CALLBACK_FUNCTION_NONE)
    (*wssl->disconnect_callback_function)(&client->connection_info);

  if(epoll_ctl(wssl->epoll_descriptor, EPOLL_CTL_DEL, client->socket_descriptor, NULL) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERRNO, "epoll_ctl", errno);

  if(close(client->socket_descriptor) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERRNO, "close", errno);
  client->socket_descriptor = WSSL_NO_DESCRIPTOR;

  wssl_chain_delete_link(&client->chain_link);

  free((void*)client);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
