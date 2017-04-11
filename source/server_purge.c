#include "main.h"

_FUNCTION_
void wssl_server_purge
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  close(server->socket_descriptor);
  server->socket_descriptor = WSSL_NO_DESCRIPTOR;

  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &server->clients)
    wssl_client_purge(wssl_client_chain_get_entry_from_server_chain_link(client_link));

  wssl_server_chain_delete_link(&server->wssl_chain_link);

  free((void*)server);
}
