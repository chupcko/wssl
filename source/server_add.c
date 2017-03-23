#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_add
(
  _WSSL_MODIFY_       wssl_t* wssl,
  _WSSL_IN_     const char*   ip,
  _WSSL_IN_     const int     port
)
{
  wssl_server_t* server = (wssl_server_t*)malloc(sizeof(wssl_server_t));
  if(server == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "server", 0);

  server->wssl = wssl;

  snprintf(server->ip, WSSL_IP_SIZE_IN_CHAR, "%s", ip);
  server->port = port;
  server->socket_descriptor = WSSL_NO_DESCRIPTOR;

  server->epoll_data.type = WSSL_EPOLL_DATA_TYPE_SERVER;
  server->epoll_data.server = server;

  wssl_client_chain_init(&server->clients);

  wssl_server_chain_add_link_backward(&wssl->servers, &server->chain_link);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
