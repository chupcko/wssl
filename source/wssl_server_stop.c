#include "main.h"

_FUNCTION_
wssl_result_t wssl_server_stop
(
  _WSSL_MODIFY_ wssl_t*        wssl,
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  wssl_chain_t* client_link;
  wssl_chain_t* client_link_next;

  if(epoll_ctl(wssl->epoll_descriptor, EPOLL_CTL_DEL, server->socket_descriptor, NULL) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);

  if(close(server->socket_descriptor) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "close", errno);
  server->socket_descriptor = WSSL_NO_DESCRIPTOR;

  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &server->clients)
    WSSL_CALL(wssl_client_delete(wssl, (wssl_client_t*)client_link));

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
