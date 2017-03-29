#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_client_send_text
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const char*          text
)
{
  wssl_frame_t frame;
  wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_TEXT, false, (wssl_octet_t*)text, (wssl_size_t)strlen(text));
  WSSL_CALL(wssl_client_send_frame(client, &frame));
  return WSSL_MAKE_RESULT_OK;
}
