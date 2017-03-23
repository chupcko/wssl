#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t client_send_binary
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_           wssl_octet_t*  data,
  _WSSL_IN_     const wssl_size_t    data_size
)
{
  wssl_frame_t frame;
  wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_CLOSE, false, data, data_size);
  WSSL_CALL(wssl_client_send_frame(client, &frame));
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
