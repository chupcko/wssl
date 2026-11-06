#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_add
(
  _WSSL_MODIFY_       wssl_t*                      wssl,
  _WSSL_IN_     const wssl_server_configuration_t* configuration
)
{
  wssl_server_t* server = (wssl_server_t*)malloc(sizeof(wssl_server_t));
  if(server == NULL)
    FAIL_ERROR("server", WSSL_RESULT_CODE_ERROR_NO_MEMORY);

  server->wssl = wssl;

  server->configuration = *configuration;

  server->socket_descriptor = WSSL_NO_DESCRIPTOR;
  server->ssl_ctx = NULL;

  server->epoll_data.type = WSSL_EPOLL_DATA_TYPE_SERVER;
  server->epoll_data.server = server;

  wssl_client_chain_root_init(&server->clients);

  wssl_server_chain_add_link_backward(&wssl->servers, &server->wssl_chain_link);

  PASS;
}
