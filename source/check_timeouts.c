#include "main.h"

static inline
wssl_result_t wssl_check_client_timeout
(
  _WSSL_MODIFY_ wssl_t*         wssl,
  _WSSL_MODIFY_ wssl_client_t*  client,
  _WSSL_IN_     wssl_mseconds_t now_in_mseconds
)
{
  if(now_in_mseconds-client->last_receive_in_mseconds >= wssl->timeout_in_mseconds)
  {
    if(wssl->on_timeout_callback == WSSL_CALLBACK_NONE)
    {
      MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_TIMEOUT);
    }
    client->last_receive_in_mseconds = now_in_mseconds;
    TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING(client, (*wssl->on_timeout_callback)(client, client->state));
  }

  PASS;
}

_FUNCTION_
wssl_result_t wssl_check_timeouts
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  if(wssl->timeout_in_mseconds == 0)
  {
    PASS;
  }

  wssl_mseconds_t now_in_mseconds = wssl_now_in_mseconds();

  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;

  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_in_wait_header)
  {
    CALL_ALLWAYS_PASS_FUNCTION(wssl_check_client_timeout(wssl, wssl_client_chain_get_entry_from_wssl_chain_link(client_link), now_in_mseconds));
  }

  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_in_frame_processing)
  {
    CALL_ALLWAYS_PASS_FUNCTION(wssl_check_client_timeout(wssl, wssl_client_chain_get_entry_from_wssl_chain_link(client_link), now_in_mseconds));
  }

  PASS;
}
