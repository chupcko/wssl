#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_loop
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  WSSL_CALL(wssl_servers_start(wssl));

  loop
    WSSL_CALL(wssl_epoll(wssl));

  WSSL_CALL(wssl_servers_stop(wssl));

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, 0);
}
