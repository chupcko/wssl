#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_add
(
  _WSSL_MODIFY_       wssl_t* wssl,
  _WSSL_IN_     const char*   ip,
  _WSSL_IN_     const int     port,
  _WSSL_IN_     const bool    ssl,
  _WSSL_IN_     const bool    ipv6
)
{
  wssl_server_t* server = (wssl_server_t*)malloc(sizeof(wssl_server_t));
  if(server == NULL)
    FAIL_ERROR("server", WSSL_RESULT_CODE_ERROR_NO_MEMORY);

  server->wssl = wssl;

  wssl_str_copy(server->ip, WSSL_IP_SIZE_IN_CHAR, ip);
  server->port = port;
  server->ssl = ssl;
  server->ipv6 = ipv6;
  server->socket_descriptor = WSSL_NO_DESCRIPTOR;

  server->epoll_data.type = WSSL_EPOLL_DATA_TYPE_SERVER;
  server->epoll_data.server = server;

  wssl_client_chain_root_init(&server->clients);

  wssl_server_chain_add_link_backward(&wssl->servers, &server->wssl_chain_link);

  PASS;
}
