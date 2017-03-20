#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_processing_frame
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_OUT_    bool*          client_deleted
)
{
  /*# odradi frame  */
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
