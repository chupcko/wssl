#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_clean
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  WSSL_CALL(wssl_servers_delete(wssl));

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
