#include "main.h"

_FUNCTION_
wssl_result_t wssl_server_stop
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  if(close(server->socket_descriptor) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERRNO, errno);
  server->socket_descriptor = WSSL_NO_DESCRIPTOR;

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, 0);
}
