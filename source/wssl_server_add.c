#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_add
(
  _WSSL_MODIFY_       wssl_t* wssl,
  _WSSL_IN_     const char*   ip,
  _WSSL_IN_     const int     port
)
{
  wssl_server_t* server;

  server = (wssl_server_t*)malloc(sizeof(wssl_server_t));
  if(server == (wssl_server_t*)NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERRNO, ENOMEM);

  snprintf(server->ip, WSSL_IP_SIZE, "%s", ip);
  server->port = port;
  server->epoll.type = WSSL_EPOLL_TYPE_SERVER;
  server->epoll.server = server;

  wssl_chain_add_link_backward(&wssl->servers, &server->link);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, 0);
}
