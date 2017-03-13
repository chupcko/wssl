#include "main.h"

_FUNCTION_
wssl_result_t wssl_servers_stop
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_chain_t* server_link;

  WSSL_CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
    WSSL_CALL(wssl_server_stop(wssl, (wssl_server_t*)server_link));

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
