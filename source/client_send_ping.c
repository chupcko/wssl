#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_client_send_ping
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  client->last_ping_in_mseconds = wssl_now_in_mseconds();
  TRY_CALL(wssl_client_send_ping_frame(client));

  PASS;
}
