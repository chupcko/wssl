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

static inline
wssl_result_t wssl_check_client_ping
(
  _WSSL_MODIFY_ wssl_client_t*  client,
  _WSSL_IN_     wssl_mseconds_t now_in_mseconds
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  if
  (
    client->ping_in_progress != true &&
    now_in_mseconds-client->last_ping_in_mseconds >= client->wssl->ping_interval_in_mseconds
  )
  {
    if(client->wssl->on_send_ping_frame_callback != WSSL_CALLBACK_NONE)
    {
      TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING_THEN_PASS(client, (*client->wssl->on_send_ping_frame_callback)(client));
    }
    TRY_CALL(wssl_client_send_ping_frame(client));
  }

  PASS;
}

static inline
wssl_result_t wssl_check_client_pong
(
  _WSSL_MODIFY_ wssl_client_t*  client,
  _WSSL_IN_     wssl_mseconds_t now_in_mseconds
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  if
  (
    client->ping_in_progress == true &&
    now_in_mseconds-client->last_ping_in_mseconds >= client->wssl->pong_timeout_in_mseconds
  )
  {
    wssl_client_mark_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_PONG_TIMEOUT);
  }

  PASS;
}

_FUNCTION_
wssl_result_t wssl_check_clients
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  if
  (
    wssl->timeout_in_mseconds == 0 &&
    wssl->ping_interval_in_mseconds == 0 &&
    wssl->pong_timeout_in_mseconds == 0
  )
  {
    PASS;
  }

  wssl_mseconds_t now_in_mseconds = wssl_now_in_mseconds();

  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;
  wssl_client_t* client;

  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_in_wait_header)
  {
    if(wssl->timeout_in_mseconds != 0)
    {
      TRY_CALL(wssl_check_client_timeout(wssl, wssl_client_chain_get_entry_from_wssl_chain_link(client_link), now_in_mseconds));
    }
  }

  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_in_frame_processing)
  {
    client = wssl_client_chain_get_entry_from_wssl_chain_link(client_link);

    if(wssl->timeout_in_mseconds != 0)
    {
      CALL_ALWAYS_PASS_FUNCTION(wssl_check_client_timeout(wssl, client, now_in_mseconds));
    }

    if(wssl->ping_interval_in_mseconds != 0 && client->ping_in_progress != true)
    {
      TRY_CALL(wssl_check_client_ping(client, now_in_mseconds));
    }

    if(wssl->pong_timeout_in_mseconds != 0)
    {
      CALL_ALWAYS_PASS_FUNCTION(wssl_check_client_pong(client, now_in_mseconds));
    }
  }

  PASS;
}
