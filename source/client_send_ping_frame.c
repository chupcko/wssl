#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_send_ping_frame
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  wssl_frame_t frame;
  wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_PING, false, WSSL_NULL, 0);
  TRY_CALL(wssl_client_send_frame(client, &frame));
  PASS;
}
