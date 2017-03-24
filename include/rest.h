#ifndef _REST_H_
#define _REST_H_

static inline
const char* wssl_get_bool_string
(
  _WSSL_IN_ const bool value
)
{
  if(value)
    return "true";
  return "false";
}

static inline
void wssl_client_set_for_disconnecting
(
  _WSSL_MODIFY_ wssl_client_t*                  client,
  _WSSL_IN_     wssl_client_disconnect_reason_e disconnect_reason
)
{
  client->state = WSSL_CLIENT_STATE_FOR_DISCONNECTING;
  client->disconnect_reason = disconnect_reason;
  wssl_client_chain_delete_link(&client->chain_link);
  wssl_client_chain_add_link_backward(&client->wssl->clients_for_disconnecting, &client->chain_link);
}

#endif
