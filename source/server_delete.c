#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_delete
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  if(server->socket_descriptor != WSSL_NO_DESCRIPTOR)
    TRY_CALL(wssl_server_stop(server));

  wssl_server_chain_delete_link(&server->chain_link);

  free((void*)server);

  return MAKE_RESULT_OK;
}
