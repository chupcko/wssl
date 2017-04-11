#include "main.h"

_LIBRARY_FUNCTION_
void wssl_purge
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  close(wssl->epoll_descriptor);
  wssl->epoll_descriptor = WSSL_NO_DESCRIPTOR;

  wssl_server_chain_t* server_link;
  wssl_server_chain_t* server_link_next;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(server_link, server_link_next, &wssl->servers)
    wssl_server_purge(wssl_server_chain_get_entry_from_wssl_chain_link(server_link));
}
