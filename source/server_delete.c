#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_delete
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  if(server->socket_descriptor != WSSL_NO_DESCRIPTOR)
    TRY_CALL(wssl_server_stop(server));

  if(server->ssl_ctx != NULL)
    SSL_CTX_free(server->ssl_ctx);

  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &server->clients)
    wssl_client_purge(wssl_client_chain_get_entry_from_server_chain_link(client_link));

  wssl_server_chain_delete_link(&server->wssl_chain_link);

  free((void*)server);

  PASS;
}
