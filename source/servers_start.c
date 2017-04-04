#include "main.h"

_FUNCTION_
wssl_result_t wssl_servers_start
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_server_chain_t* server_link;
  CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
    TRY_CALL(wssl_server_start(wssl_server_chain_get_entry_from_chain_link(server_link)));
  PASS;
}
