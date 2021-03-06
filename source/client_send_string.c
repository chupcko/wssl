#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_client_send_string
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_string_t* string
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  wssl_frame_t frame;
  wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_TEXT, false, (wssl_octet_t*)string->data, string->data_length);
  TRY_CALL(wssl_client_send_frame(client, &frame));
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
  PASS;
}
