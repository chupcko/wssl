#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_clean
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_marked_for_disconnecting)
    TRY_CALL(wssl_client_delete(wssl_client_chain_get_entry_from_chain_link(client_link)));

  wssl_server_chain_t* server_link;
  wssl_server_chain_t* server_link_next;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(server_link, server_link_next, &wssl->servers)
    TRY_CALL(wssl_server_delete(wssl_server_chain_get_entry_from_chain_link(server_link)));

  return MAKE_RESULT_OK;
}
