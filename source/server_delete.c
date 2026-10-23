#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_delete
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  if(server->socket_descriptor != WSSL_NO_DESCRIPTOR)
    TRY_CALL(wssl_server_stop(server));

  if(server->ssl_ctx != WSSL_NULL)
  {
    SSL_CTX_free(server->ssl_ctx);
    server->ssl_ctx = WSSL_NULL;
  }

  wssl_server_chain_delete_link(&server->wssl_chain_link);

  free((void*)server);

  PASS;
}
