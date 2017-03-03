#include "main.h"

_LIBRARY_FUNCTION_
void wssl_clean
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_chain_t* server_link;
  wssl_chain_t* server_link_next;
  wssl_server_t* server;

  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(server_link, server_link_next, &wssl->servers)
  {
    wssl_chain_delete_link(server_link);
    server = (wssl_server_t*)server_link;
    free((void*)server);
  }
}
