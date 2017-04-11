#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_clean
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_server_chain_t* server_link;
  wssl_server_chain_t* server_link_next;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(server_link, server_link_next, &wssl->servers)
    TRY_CALL(wssl_server_delete(wssl_server_chain_get_entry_from_wssl_chain_link(server_link)));

  WSSL_ASSERT(wssl_client_chain_is_empty(&wssl->clients_in_wait_header));
  WSSL_ASSERT(wssl_client_chain_is_empty(&wssl->clients_in_frame_processing));
  WSSL_ASSERT(wssl_client_chain_is_empty(&wssl->clients_marked_for_disconnecting));

  PASS;
}
