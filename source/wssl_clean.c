#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_clean
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_chain_t* server_link;
  wssl_chain_t* server_link_next;

  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(server_link, server_link_next, &wssl->servers)
    WSSL_CALL(wssl_server_delete(wssl, (wssl_server_t*)server_link));

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
