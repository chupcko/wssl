#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_send_ping_frame
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  wssl_size_t i;
  for(i = 0; i < WSSL_PING_CONTEXT_SIZE; i++)
  {
    client->ping_context[i] = wssl_random_octet(client->wssl);
  }

  wssl_frame_t frame;
  wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_PING, false, client->ping_context, WSSL_PING_CONTEXT_SIZE);
  TRY_CALL(wssl_client_send_frame(client, &frame));
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  client->last_ping_in_mseconds = wssl_now_in_mseconds();
  client->ping_in_progress = true;

  PASS;
}
