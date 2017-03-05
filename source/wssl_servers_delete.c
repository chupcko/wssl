#include "main.h"

_FUNCTION_
wssl_result_t wssl_servers_delete
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_chain_t* server_link;
  wssl_chain_t* server_link_next;
  wssl_server_t* server;

  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(server_link, server_link_next, &wssl->servers)
  {
    server = (wssl_server_t*)server_link;
    WSSL_CALL(wssl_server_delete(wssl, server));
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
