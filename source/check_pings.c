#include "main.h"

static inline
wssl_result_t wssl_check_client_ping
(
  _WSSL_MODIFY_ wssl_client_t*  client,
  _WSSL_IN_     wssl_mseconds_t now_in_mseconds
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  if(now_in_mseconds-client->last_ping_in_mseconds >= client->wssl->ping_interval_in_mseconds)
  {
    if(client->wssl->on_send_ping_frame_callback != WSSL_CALLBACK_NONE)
    {
      TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING_THEN_PASS(client, (*client->wssl->on_send_ping_frame_callback)(client));
    }
    client->last_ping_in_mseconds = now_in_mseconds;
    TRY_CALL(wssl_client_send_ping_frame(client));
  }

  PASS;
}

_FUNCTION_
wssl_result_t wssl_check_pings
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  if(wssl->ping_interval_in_mseconds == 0)
  {
    PASS;
  }

  wssl_mseconds_t now_in_mseconds = wssl_now_in_mseconds();

  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;

  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_in_frame_processing)
  {
    TRY_CALL(wssl_check_client_ping(wssl_client_chain_get_entry_from_wssl_chain_link(client_link), now_in_mseconds));
  }

  PASS;
}
