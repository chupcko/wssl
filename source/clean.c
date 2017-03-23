#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_clean
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;
  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_for_disconnecting)
    WSSL_CALL(wssl_client_delete(wssl_client_chain_entry(client_link)));

  wssl_server_chain_t* server_link;
  wssl_server_chain_t* server_link_next;
  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(server_link, server_link_next, &wssl->servers)
    WSSL_CALL(wssl_server_delete(wssl_server_chain_entry(server_link)));

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
